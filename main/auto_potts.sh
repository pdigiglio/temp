source ./variables.sh

# Create directory
mkdir --parents --verbose ${tdir}

#--------------------------------------------------------------------
# MAKE EXCUTABLES
#--------------------------------------------------------------------

# Make ising excutable
make ising_potts

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
./ising_potts


# Susceptivity and specific heath
SUS=sus
CV=cv

./susc mag_sweep_abs.dat > ${SUS}_sweep.dat
./susc mag_cluster.dat > ${SUS}_cluster.dat
./susc energia.dat > ${CV}.dat


# Plot magnetization dispersion
gnuplot -e "filename='mag_sweep'" mag_sweep_potts.gpl
mv mag_sweep.{dat,png} --target-directory=${tdir}

# Observable analysis
for file in energia mag_sweep_abs mag_cluster ${SUS}_sweep ${CV} ${SUS}_cluster sus_improved
do
	echo "[`tput setaf 4`${file}`tput sgr0`: `tput setaf 3`auto-correlator`tput sgr0`]"
	./analisi ${file}.dat
	mv --verbose --update ac.dat ac_${file}.dat

	# Generate plot
	gnuplot -e "fn='${file}'" plot.gpl
	# Generate histogram
	gnuplot -e "filename='${file}';bins='100'" histogram.gpl


	echo "[`tput setaf 3`binning`tput sgr0`]"
	./binning ${file}.dat ${bin_lim} > bin_${file}.dat

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
for (( i = 1; i < ${bin_lim}; i ++ ))
do
	echo "[`tput setaf 4`corr_length`tput sgr0`] binning >> ${i} / ${bin_lim}"
	./corr_length corr.dat ${i}
done

# Create correlator length binning plot
gnuplot corr_length.gpl
