##### Supported (checked) #####
 1) Microsoft Visual Studio <..> 2022 (64-bit) - Version 17.2.0 and (probably) later
 2) Microsoft Visual Studio <..> 2022 PREVIEW (64-bit) - Preview 3.0, 4.0 and (for sure) later

##### HowTo Build ##### 
 1) Create System Level Environment variable `BmiCacheDir` with the value of named modules directory name, eg: `BmiCacheDir=C:\BmiCacheDir\`  
 2) Open Demo-App solution and agree to retarget it all to the latest Windows SDK Version and Platform Toolset  
 3) Build projects from `prebuilt` group (C++20 named modules)  
 4) Build demo with its own C++20 named modules  

Big thanks to the author of these projects for the educational material.
