dl_theta(l,w0) =  l/pi/w0
dl_w0(l,theta) =  l/pi/theta
omega(z,w0,theta) = sqrt( w0**2 + z**2*theta**2)
w0(w,z,theta) = sqrt(w**2 - z**2*theta**2)

set xlabel "[um]"
set ylabel "[um]"
set xrange[-5:5]

wavelength  = 500
w0 = 10
theta = dl_theta(wavelength,w0)
M2 = 2

set term png
set output 'two-beams-with-same-waist.png'
plot omega(x,w0,theta) lt -1 lw 2 title "M^2 = 1",-omega(x,w0,theta) lt -1 lw 2 title "", omega(x,w0,M2*theta) lt 0 lw 2 title "M^2 = 2", -omega(x,w0,M2*theta) lt 0 lw 2 title ""


set output 'two-beams-with-same-divergence.png'
plot omega(x,w0,theta) lt -1 lw 2 title "M^2 = 1",-omega(x,w0,theta) lt -1 lw 2 title "", omega(x,M2*w0,theta) lt 0 lw 2 title "M^2 = 2", -omega(x,M2*w0,theta) lt 0 lw 2 title ""


