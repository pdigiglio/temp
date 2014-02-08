# Temperature
B=.9
# Lattice side
side=200
tdir="./B_${B}"

# Create directory
mkdir --verbose ${tdir}

#--------------------------------------------------------------------
# MAKE EXCUTABLES
#--------------------------------------------------------------------

# Make ising excutable
make ising

# Compile auto-correlator routine
make analisi

# Compile fit routine
make ac_fit
# Compile binning

make binning

# (To) Generate susceptivity and specific heath data files
make susc


#--------------------------------------------------------------------
# GENERATE DATA
#--------------------------------------------------------------------

# Make data files
./ising


# Susceptivity and specific heath
SUS=sus
CV=cv

./susc mag_sweep_abs.dat > ${SUS}_sweep.dat
./susc mag_cluster.dat > ${SUS}_cluster.dat
./susc energia.dat > ${CV}.dat


# Observable analysis
for file in energia mag_cluster mag_sweep_abs mag_sweep ${SUS}_sweep ${SUS}_cluster ${CV}
do
	echo "[`tput setaf 4`${file}`tput sgr0`: `tput setaf 3`auto-correlator`tput sgr0`]"
	./analisi ${file}.dat
	mv --verbose --update ac.dat ac_${file}.dat

	# Generate plot
	gnuplot -e "fn='${file}'" plot.gpl
	# Generate histogram
	gnuplot -e "filename='${file}';bins='80'" histogram.gpl


	echo "[`tput setaf 3`binning`tput sgr0`]"
	./binning ${file}.dat 30 > bin_${file}.dat

	# Generate plot
	gnuplot -e "fn='bin_${file}'" binning.gpl
	
	echo "[`tput setaf 3`fit`tput sgr0`]"
	./ac_fit ac_${file} ${B} >> ac_${file}_times.dat

	mv --verbose --update \
		ac_${file}.{dat,png} \
		${file}_plot.png \
		${file}{,_histo}.{dat,png} \
		bin_${file}.dat bin_${file}_{var,sdom}.png \
		--target-directory=${tdir}
done

# Evaluate correlation length
echo "[`tput setaf 4`corr_length`tput sgr0`]"
make corr_length

# correlation length binning
for (( i = 1; i < 30; i ++ ))
do
	./corr_length corr.dat ${i}
done

# Extract plot
line=10
for (( i = 0; i < side / 2; i ++ ))
do
	echo -e "${i}\t`sed -e "/^#/d" cl_binning.dat | head -n ${line} | tail -n 1 | cut -f $[ 4 * $i + 4 ],$[ 4 * $i + 5 ]`" >> cl_plot.dat
done

echo "[`tput setaf 3`fit`tput sgr0`]"
./ac_fit cl_plot ${B} >> correlation_length.dat

mv --update --verbose corr.dat cl_binning.{dat,png} cl_plot.{dat,png} --target-directory=${tdir}
