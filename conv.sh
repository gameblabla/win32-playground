ffmpeg -i red.png -c:v rawvideo -pix_fmt rgb555le output.rgb
# bgr0 is for the RGB32 GDI mode
#ffmpeg -i s.jpg -c:v rawvideo -pix_fmt bgr0 output.rgb
bin2c output.rgb s.h pict
