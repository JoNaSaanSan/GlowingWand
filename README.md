# GlowingWand
For some reasons the sklearn-bayes library only work with the ubuntu shell... donno why

**How to train a model:**
install ubuntu shell on windows 10
https://www.windowscentral.com/install-windows-subsystem-linux-windows-10

within ubuntu shell
install packages (make sure python3 and pip is installed)
- cython
- numpy 
https://github.com/NiklasRosenstein/sklearn-bayes
- micromlgen

put all csv. (gesture) files extracted from adruino into ML/datasets directory
run classifiergenerator (python3 classifiergenerator.py)

copy paste predict, compute kernel and classIdxToName (could be predictLabel -> just rename to classIdxToName) functions into model.h which is located in the Arduino/glowing_wand directory

model can now be used - use classify to detect gestures

**How to create csv. files**
connect glowin wand to computer and open serial monitor
comment out classify() 
use printfeatures() to detect movement
copy paste the gesture data into a csv file... one csv file for each gesture
