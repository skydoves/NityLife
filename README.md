# NityLife
Cocos-2dx c++ win32 and mobile patform game in 2014 main codes.


![screenshot_2014-08-17-00-13-28](https://user-images.githubusercontent.com/24237865/27590756-9cbfe788-5b8a-11e7-8bae-ccda2999c913.png)

<a href='https://play.google.com/store/apps/details?id=com.skydoves.nitylife&pcampaignid=MKT-Other-global-all-co-prtnr-py-PartBadge-Mar2515-1'><img alt='Get it on Google Play' src='https://developer.android.com/images/brand/en_generic_rgb_wo_60.png'/></a>

## Prerequisites:

* python 2.7.x
* Cocos2d-x v3.11.1
* Android NDK
* Android SDK **OR** Eclipse ADT Bundle
* Android AVD target installed
* Android Gingerbread 2.3 api level 10 (eclipse project)

## Building project on Android

There are two ways of building Android projects.

1. Eclipse
2. Command Line

### Import Project in Eclipse

####Step 1: C/C++ Environment Variable `NDK_ROOT`

* Eclipse->Preferences->C/C++->Build->**Environment**.
* Click **Add** button and add a new variable `NDK_ROOT` pointing to the root NDK directory.
	![Example](https://lh3.googleusercontent.com/-AVcY8IAT0_g/UUOYltoRobI/AAAAAAAAsdM/22D2J9u3sig/s400/cocos2d-x-eclipse-ndk.png)
* Only for Windows: Add new variables **CYGWIN** with value `nodosfilewarning` and **SHELLOPTS** with value `igncr`

####Step 2: Adding and running from Eclipse

![Example](https://lh3.googleusercontent.com/-SLBOu6e3QbE/UUOcOXYaGqI/AAAAAAAAsdo/tYBY2SylOSM/s288/cocos2d-x-eclipse-project-from-code.png) ![Import](https://lh5.googleusercontent.com/-XzC9Pn65USc/UUOcOTAwizI/AAAAAAAAsdk/4b6YM-oim9Y/s400/cocos2d-x-eclipse-import-project.png)

1. File->New->Project->Android Project From Existing Code
2. **Browse** to your project directory and Add the project 
3. Click **Run as Android Application** to run on connected device or emulator.

That's all !!! 

### Running project from Command Line

    $ cd NEW_PROJECTS_DIR/MyGame
    $ cocos run -p android -j 4

# License
```xml
Copyright 2017 skydoves

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
