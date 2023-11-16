urdf2rai.py pr2.urdf -meshExt 'ply' > z.1.g

echo "Edit base_link_0 { meshscale: 0.1 }" >>z.1.g

kinEdit -file z.1.g -prune -cleanOnly
mv z.g pr2_clean.g
