urdf2rai.py panda_arm_hand.urdf -meshExt 'ply' > z.1.g
kinEdit -file z.1.g -cleanOnly
mv z.g panda_clean.g
