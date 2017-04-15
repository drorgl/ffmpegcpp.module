{
	'variables':{
		'shared_runtime%': 'true',
	},
	'target_defaults' : {
		 # enable exceptions for all targets
		
		'conditions' : [
			['1==1', {
					'configurations': {
						'Release':{
							'target_conditions':[
								['1==1',{
									'msvs_settings': {
										'VCCLCompilerTool': {
											'RuntimeTypeInfo': 'true',
											'RuntimeLibrary': 2, # MultiThreadedDLL (/MD)
											#'AdditionalOptions' :['/MD']
										}
									}
								}],
								['"true"=="true"', {
									'msvs_settings': {
										'VCCLCompilerTool': {
											'RuntimeLibrary': 2, # MultiThreadedDLL (/MD)
											'AdditionalOptions' :['/MD']
										}
									}
								}]
							]
						},
						'Debug':{
							'target_conditions':[
								['1==1',{
									'msvs_settings':{
										'VCCLCompilerTool': {
											'RuntimeTypeInfo': 'true',
											'RuntimeLibrary': 3, # MultiThreadedDebugDLL (/MDd)
											#'AdditionalOptions' :['/MDd']
										 }
									}
								}]
							]
						}
					 },
			
			
					'msvs_settings' : {
						'VCCLCompilerTool' : {
							'WarningLevel' : 0,
							'WholeProgramOptimization' : 'false',
							'RuntimeTypeInfo': 'true',
							'AdditionalOptions' : ['/EHsc'],
							'ExceptionHandling' : 1,
							 #  / EHsc
						}
					},
					'defines!' : [
						'_HAS_EXCEPTIONS=0',
						'_HAS_EXCEPTIONS',
					]
					
				}
			],
			
			['OS in "win"',{
				'defines' : [
					'DELAYIMP_INSECURE_WRITABLE_HOOKS'
				]
			}],
			
			['OS in "linux freebsd openbsd solaris android aix"', {
					'cflags_cc!' : ['-fno-rtti', '-fno-exceptions', '-std=gnu++0x', '-std=c++11', '-Wno-sign-compare'],
					'cflags_cc' : ['-std=c++14'],
					'ldflags':[
						'-Wl,-rpath=\$$ORIGIN',
						'-Wl,-rpath-link=<(PRODUCT_DIR)/'
					],
			}],
			['OS=="mac"', {
					'xcode_settings' : {
						'GCC_ENABLE_CPP_EXCEPTIONS' : 'YES',
						 # -fno - exceptions
					}
					
			}]
		],
	}
	
}
