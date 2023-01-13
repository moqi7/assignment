%IIR
%mstg  st
N = 1600;       
Fs = 10000;       
T = 1/Fs;
Tp = N*T;       
t = 0:T:(N-1)*T;
k = 0:N-1;
f = k/Tp;
fc1 = Fs/10;      
fm1 = fc1/10;      
fc2 = Fs/20;      
fm2 = fc2/10;
fc3 = Fs/40;
fm3 = fc3/10;
xt1 = cos(2*pi*fm1*t).*cos(2*pi*fc1*t);
xt2 = cos(2*pi*fm2*t).*cos(2*pi*fc2*t);
xt3 = cos(2*pi*fm3*t).*cos(2*pi*fc3*t);
st = xt1+xt2+xt3; 
fxt = fft(st,N); 

subplot(2,1,1);  
plot(t,st);
grid;
title('s(t)waveform');
xlabel('t/s');ylabel('s(t)');
axis([0,Tp/8,min(st),max(st)]);

subplot(2,1,2);
stem(f,abs(fxt)/max(abs(fxt)),'.');
grid;
title('s(t)spectrum');
xlabel('f/Hz');ylabel('range');
axis([0,Fs/5,0,1.2]);


%LPF
fp = 280;     
fs = 450;      
wp = 2*fp/Fs;     
ws = 2*fs/Fs;
rp = 0.1;
rs = 60;
[N,wp] = ellipord(wp,ws,rp,rs); 
[B,A] = ellip(N,rp,rs,wp);   
y1t = filter(B,A,st);

figure(2)
subplot(3,2,1); 
[H,W] = freqz(B,A,1000);
m = abs(H);
plot(W/pi,20*log10(m/max(m)));
grid;
title('Low-pass filter loss function curve');
xlabel('\omega/\pi');ylabel('range');
axis([0,1,-80,5]);

subplot(3,2,2);     
n = 0:length(y1t)-1;
t = n*T;
plot(t,y1t);
title('y1(t) temporal waveform ');
xlabel('t/s');ylabel('y1t(t)');
axis([0,t(end),min(y1t),1.2*max(y1t)]);


%Bandpass filter
fpl = 440;
fpu = 560;
fsl = 275;
fsu = 900;
wp = [2*fpl/Fs,2*fpu/Fs];    
ws = [2*fsl/Fs,2*fsu/Fs];    
[N,wp] = ellipord(wp,ws,rp,rs);     
[B,A] = ellip(N,rp,rs,wp);       
y2t = filter(B,A,st);

subplot(3,2,3);          
[H,W] = freqz(B,A,1000);
m = abs(H);
plot(W/pi,20*log10(m/max(m)));
grid;
title('Bandpass filter loss function curve');
xlabel('\omega/\pi');ylabel('range');
axis([0,1,-80,5]);

subplot(3,2,4);   
n = 0:length(y2t)-1;
t = n*T;
plot(t,y2t);
title('y2(t) temporal waveform ');
xlabel('t/s');ylabel('y2(t)');
axis([0,t(end),min(y2t),1.2*max(y2t)]);


%High-pass filter
fp = 890;
fs = 600;
wp = 2*fp/Fs;   
ws = 2*fs/Fs;
rp = 0.1;
rs = 60;
[N,wp] = ellipord(wp,ws,rp,rs);  
[B,A] = ellip(N,rp,rs,wp,'high');   
y3t = filter(B,A,st);

subplot(3,2,5);     
[H,W] = freqz(B,A,1000);
m = abs(H);
plot(W/pi,20*log10(m/max(m)));
grid;
title('High-pass filter loss function curve');
xlabel('\omega/\pi');ylabel('range');
axis([0,1,-80,5]);

subplot(3,2,6); 
n = 0:length(y3t)-1;
t = n*T;
plot(t,y3t);
title('y3(t) temporal waveform');
xlabel('t/s');ylabel('y3(t)');
axis([0,t(end),min(y3t),1.2*max(y3t)]);
