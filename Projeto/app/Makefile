#
#  Configure:
#  source /opt/yocto/poky/1.3.2/environment-setup-core2-poky-linux
#

main: main.cpp
	$ g++ main.cpp classe.cpp -o main -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_dialog -lallegro_acodec
	# cp app /opt/tftp

clean:
	rm -rf *.o *~ core main
