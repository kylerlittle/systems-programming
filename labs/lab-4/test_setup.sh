mkdir test
cd test
mkdir d1 d2 d1/d3
touch d2/f2 f1
echo "adding some bullshit text" > f1
ln f1 link1
ln -s f1 slink
cd ..
