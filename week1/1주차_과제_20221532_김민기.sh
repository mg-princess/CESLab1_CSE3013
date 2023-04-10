echo "working directory:"
read dir
if [ ! -d $dir ]; then 
    echo "Wrong directory"
    exit 1
fi

if [ "$dir" != "" ]; then
    echo "Moving directory..."
    cd $dir
    if [ $? -ne 0 ]; then 
        echo "Cannot access directory"
        exit 1
    fi 
fi

for name in *; do
    mv $name `echo $name | tr '[A-Z] [a-z]' '[a-z] [A-Z]'`
done
