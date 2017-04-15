# ffmpegcpp gyp project
# Copyright (c) 2015 Dror Gluska, All rights reserved.
#
# This file is part of ffmpegcpp
#
# ffmpegcpp is a wrapper for ffmpeg and it's a free software under the 3-clause BSD license
#
# ffmpegcpp is distributed in the hope that it will be useful but WITHOUT ANY WARRANTY.

{
	'variables':{
		'library' : 'static_library',
		#'library' : 'shared_library',
	},
	'target_defaults': {
		'win_delay_load_hook': 'false',
		'msvs_settings': {
			# This magical incantation is necessary because VC++ will compile
			# object files to same directory... even if they have the same name!
			'VCCLCompilerTool': {
			  'ObjectFile': '$(IntDir)/%(RelativeDir)/',
			  #'AdditionalOptions': [ '/EHsc', '/wd4244']
			  'WarningLevel': 0,
			  'WholeProgramOptimization': 'false',
			  'AdditionalOptions': ['/EHsc'],
			  'ExceptionHandling' : 1, #/EHsc
			},
			
		},
		'configurations':{
			'Debug':{
				'conditions': [
				  ['target_arch=="x64"', {
					'msvs_configuration_platform': 'x64',
				  }],
				  ['1==1',{

					'defines':[
						'DEBUG',
					],
					'msvs_settings': {		
						'VCCLCompilerTool': {
						  #'WholeProgramOptimization' : 'false',
						  #'AdditionalOptions': ['/GL-','/w'], #['/wd4244' ,'/wd4018','/wd4133' ,'/wd4090'] #GL- was added because the forced optimization coming from node-gyp is disturbing the weird coding style from ffmpeg.
						  'WarningLevel': 0,
						  'WholeProgramOptimization': 'false',
						  'AdditionalOptions': ['/EHsc'],
						  'ExceptionHandling' : 1, #/EHsc
						  'RuntimeLibrary': 3, # dll debug
						},
						'VCLinkerTool' : {
							'GenerateDebugInformation' : 'true',
							'conditions':[
								['target_arch=="x64"', {
									'TargetMachine' : 17 # /MACHINE:X64
								}],
							],
							
						}
					}
				
				  }],
				],
				
			},
			'Release':{
				'conditions': [
				  ['target_arch=="x64"', {
					'msvs_configuration_platform': 'x64',
				  }],
				],
				'msvs_settings': {			
					'VCCLCompilerTool': {
						'WholeProgramOptimization' : 'false',
						#'AdditionalOptions': ['/GL-','/w'], #['/wd4244' ,'/wd4018','/wd4133' ,'/wd4090'] #GL- was added because the forced optimization coming from node-gyp is disturbing the weird coding style from ffmpeg.
						'WarningLevel': 0,
						  'WholeProgramOptimization': 'false',
						  'AdditionalOptions': ['/EHsc'],
						  'ExceptionHandling' : 1, #/EHsc
						  'RuntimeLibrary': 2, # dll release
					},
					'VCLinkerTool' : {
						'conditions':[
							['target_arch=="x64"', {
								'TargetMachine' : 17 # /MACHINE:X64
							}],
						],
						
					}
				}
			},
		},
		
		'conditions': [
			['OS == "win"',{
				'defines':[
					'DELAYIMP_INSECURE_WRITABLE_HOOKS'
				]
			}],
		  ['OS != "win"', {
			'defines': [
			  '_LARGEFILE_SOURCE',
			  '_FILE_OFFSET_BITS=64',
			  
			],
			'cflags':[
				'-fPIC',
				'-std=c++11',
				'-fexceptions',
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ],
			'conditions': [
				['OS=="mac"', {
				  'xcode_settings': {
					'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
				  }
				}]
			],
			'conditions': [
			  ['OS=="solaris"', {
				'cflags': [ '-pthreads' ],
			  }],
			  ['OS not in "solaris android"', {
				'cflags': [ '-pthread' ],
			  }],
			],
		}],
		['OS=="android"',{
			'defines':[
				'ANDROID'
			],
		  }],
		],
	  },
	'targets':
	[
		{
			'target_name': 'ffmpegcpp',
			'type':'<(library)',
			'dependencies':[
				#"<!(node -e \"require('ffmpeg.module')\"):avcodec",
				#"<!(node -e \"require('ffmpeg.module')\"):avdevice",
				#"<!(node -e \"require('ffmpeg.module')\"):avfilter",
				#"<!(node -e \"require('ffmpeg.module')\"):avformat",
				#"<!(node -e \"require('ffmpeg.module')\"):avresample",
				#"<!(node -e \"require('ffmpeg.module')\"):avutil",
				#"<!(node -e \"require('ffmpeg.module')\"):postproc",
				#"<!(node -e \"require('ffmpeg.module')\"):swresample",
				#"<!(node -e \"require('ffmpeg.module')\"):swscale",
#
				#"<!(node -e \"require('opencv.module')\"):core",
				#"<!(node -e \"require('alsa-lib.module')\"):alsa-lib",
				#"<!(node -e \"require('v4l-utils.module')\"):v4l2",

				'<!@(nnbu-dependency --dependency ffmpeg)',
				'<!@(nnbu-dependency --dependency opencv)',
				'<!@(nnbu-dependency --dependency v4l)',
				'<!@(nnbu-dependency --dependency alsa-lib)'
				#'../ffmpeg.module/ffmpeg.gyp:avcodec',
				#'../ffmpeg.module/ffmpeg.gyp:avdevice',
				#'../ffmpeg.module/ffmpeg.gyp:avfilter',
				#'../ffmpeg.module/ffmpeg.gyp:avformat',
				#'../ffmpeg.module/ffmpeg.gyp:avresample',
				#'../ffmpeg.module/ffmpeg.gyp:avutil',
				#'../ffmpeg.module/ffmpeg.gyp:postproc',
				#'../ffmpeg.module/ffmpeg.gyp:swresample',
				#'../ffmpeg.module/ffmpeg.gyp:swscale',
				#'../opencv.module/opencv.gyp:core',
				#'../alsa-lib.module/alsa-lib.gyp:alsa-lib',
				#'../v4l-utils.module/v4l-utils.gyp:v4l2',
			],
			
			'include_dirs':[
				'ffmpegcpp/includes',

				'<!@(nnbu-dependency --headers ffmpeg)',
				'<!@(nnbu-dependency --headers opencv)',
				'<!@(nnbu-dependency --headers v4l)',
				'<!@(nnbu-dependency --headers alsa-lib)'
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'ffmpegcpp/includes'
				],
			 },
			 'link_settings':{
					'libraries':[
						'<!@(nnbu-dependency --lib-fix --libs ffmpeg)',
						'<!@(nnbu-dependency --lib-fix --libs opencv)',
						'<!@(nnbu-dependency --lib-fix --libs v4l)',
						'<!@(nnbu-dependency --lib-fix --libs alsa-lib)'
					],
			 },
			 
			 'copies':[
				 {
					 'destination':'<(PRODUCT_DIR)',
					 'files':[
						 		'<!@(nnbu-dependency --copy ffmpeg)',
					 ]
				 }
			 ],

			'sources':[
				'ffmpegcpp/includes/codec.h',
				'ffmpegcpp/includes/formatcontext.h',
				'ffmpegcpp/includes/datastructures.h',
				'ffmpegcpp/includes/decoder.h',
				'ffmpegcpp/includes/dictionary.h',
				'ffmpegcpp/includes/encoder.h',
				'ffmpegcpp/includes/exports.h',
				'ffmpegcpp/includes/ffmpeg.h',
				'ffmpegcpp/includes/ffmpeg_exception.h',
				'ffmpegcpp/includes/frame.h',
				'ffmpegcpp/includes/packet.h',
				'ffmpegcpp/includes/stdafx.h',
				'ffmpegcpp/includes/stream.h',
				'ffmpegcpp/includes/swscale.h',
				'ffmpegcpp/includes/memorycontext.h',
				'ffmpegcpp/includes/bitstreamfilter.h',
				'ffmpegcpp/src/codec.cpp',
				'ffmpegcpp/src/codec_pimpl.h',
				'ffmpegcpp/src/formatcontext.cpp',
				'ffmpegcpp/src/formatcontext_pimpl.h',
				'ffmpegcpp/src/datastructures.cpp',
				'ffmpegcpp/src/datastructures_conversion.h',
				'ffmpegcpp/src/decoder.cpp',
				'ffmpegcpp/src/decoder_pimpl.h',
				'ffmpegcpp/src/dictionary.cpp',
				'ffmpegcpp/src/dictionary_pimpl.h',
				'ffmpegcpp/src/encoder.cpp',
				'ffmpegcpp/src/ffmpeg.cpp',
				'ffmpegcpp/src/ffmpeg_exception.cpp',
				'ffmpegcpp/src/ffmpeg_includes.h',
				'ffmpegcpp/src/frame.cpp',
				'ffmpegcpp/src/frame_pimpl.h',
				'ffmpegcpp/src/linux',
				'ffmpegcpp/src/packet.cpp',
				'ffmpegcpp/src/packet_pimpl.h',
				'ffmpegcpp/src/stream.cpp',
				'ffmpegcpp/src/stream_pimpl.h',
				'ffmpegcpp/src/stringutils.h',
				'ffmpegcpp/src/swscale.cpp',
				'ffmpegcpp/src/iobuffer.cpp',
				'ffmpegcpp/src/iobuffer.h',
				'ffmpegcpp/src/memorycontext.cpp',
				'ffmpegcpp/src/memorycontext_pimpl.h',
				'ffmpegcpp/src/bitstreamfilter.cpp',
				'ffmpegcpp/src/bitstreamfilter_pimpl.h',
			],
			'conditions':[
				['OS == "win" and library == "shared_library"',{
					'defines': [
					  'FFMPEGCPP_DLL_EXPORT',
					],
				}],
				['OS in "linux android"',{
					'sources':[
						'ffmpegcpp/src/linux/alsa/alsawrapper.cpp',
						'ffmpegcpp/src/linux/alsa/alsawrapper.h',
						'ffmpegcpp/src/linux/v4l2/v4l2_wrapper.cpp',
						'ffmpegcpp/src/linux/v4l2/v4l2_wrapper.h',
					],
					
				}],
				
			]
		},
		
		{
			'target_name': 'ffmpegcpp.sample',
			'type':'executable',
			'dependencies':[
				'ffmpegcpp',
				#"<!(node -e \"require('opencv.module')\"):core",
				'<!@(nnbu-dependency --dependency opencv)',
				#'../opencv.module/opencv.gyp:core',
				#'../opencv.module/opencv.gyp:hal',
			],
			'defines':[],
			'include_dirs':[
				'<!@(nnbu-dependency --headers opencv)',
			],
			'includes':[
			 ],
			 'link_settings':{
					'libraries':[
						'<!@(nnbu-dependency --lib-fix --libs opencv)',
					],
			 },



			'sources':[
				'ffmpegcpp.sample/ffmpegcpp.sample.cpp',
				'ffmpegcpp.sample/Tests/all.h',
				'ffmpegcpp.sample/Tests/catch.hpp',
				'ffmpegcpp.sample/Tests/codectests.h',
				'ffmpegcpp.sample/Tests/datastructurestest.h',
				'ffmpegcpp.sample/Tests/dictionarytest.h',
				'ffmpegcpp.sample/Tests/ffmpegtests.h',
				'ffmpegcpp.sample/Tests/frametests.h',
				'ffmpegcpp.sample/Tests/memorycontexttest.h',
				'ffmpegcpp.sample/Tests/packettests.h',		
				'ffmpegcpp.sample/Tests/encodingdecodingtests.h',		
				'ffmpegcpp.sample/Tests/filtertests.h',
			],
			'conditions':[
				['OS == "win" and library == "shared_library"',{
					'defines': [
					  'FFMPEGCPP_DLL_IMPORT',
					],
				}],
			]
		},
	]
}