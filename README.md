# GlowingWand
install ubuntu shell on windows 10
https://www.windowscentral.com/install-windows-subsystem-linux-windows-10

within ubuntu shell
install packages (make sure python3 and pip is installed)
- cython
- numpy 
https://github.com/NiklasRosenstein/sklearn-bayes
- micromlgen

put csv. files extracted from adruino into ML/datasets directory
run classifiergenerator (python3 classifiergenerator.py)

copy paste predict, compute kernel and classIdxToName (could be predictLabel -> just rename to classIdxToName) functions into model.h which is located in the Arduino/glowing_wand directory

model can now be used - use classify to detect gestures

*How to create csv. files*
comment out classify() 
use printfeatures() to detect movement
