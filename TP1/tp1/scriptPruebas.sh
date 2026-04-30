#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH --partition=Blade
#SBATCH -o /nethome/spusuario9/tp1/output.txt
#SBATCH -e /nethome/spusuario9/tp1/errors.txt
#SBATCH --time=00:05:00 #Tiempo límite (HH:MM:SS)
./Ej2Exe $1 $2