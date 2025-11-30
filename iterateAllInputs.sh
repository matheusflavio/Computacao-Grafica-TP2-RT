rm demo

rm -rf exec_out images_out

mkdir exec_out images_out

make -j12

for inputfile in ./inputs/*.*; do
    nameFile="${inputfile#./inputs/}"      # Remove a parte './inputs/' do caminho
    outputfile="${nameFile%.*}"             # Remove a extensão do nome do arquivo

    # Chama o programa com o nome do arquivo de saída sem a extensão
    ./demo "$inputfile" "./exec_out/$outputfile"
    ffmpeg -i "./exec_out/$outputfile.ppm" "./images_out/${nameFile%.*}.png"
done
