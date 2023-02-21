/**
* JetBrains Space Automation
* This Kotlin-script file lets you automate build activities
* For more info, see https://www.jetbrains.com/help/space/automation.html
*/


    job("build-ubuntu") {
    	container("ubuntu")
        {
        	  
                shellScript
                {
                	content = """
                 	cmake -B ./build
              
                 	cmake --build ./build
                 """
                }
          	  
        }
        }
    job("build-alpine"){
        container("alpine")
        {
        	 
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 
                 	cmake --build ./build
                 """
                }
          	  
        }}
    job("build-manjaro"){
        container("manjaro")
        {
        	  
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 	cmake --build ./build
                 """
                }
          	  
        }}
    job("build-windows"){
        container("macos")
        {
        	 
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 
                 	cmake --build ./build
                 """
                }
          	  
        }}
    job("build-windows"){
        container("windows")
        {
        	  
                shellScript
                {
                	content = """
                 	cmake -B ./build
                 
                 	cmake --build ./build
                 """
                }
          	  
        }
    }

