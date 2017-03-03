clc;clear all;
Fs=128;
filt_n=4;
[filter_b,filter_a]=butter(filt_n,2/(Fs/2),'high');%4阶巴特沃斯高通滤波器

DATA_LENGTH=200*Fs;
DATA_CHANNEL=8;
ch=[1,3,5,7];%指前额AF3,AF4,F3,F4,但是Emotiv反着戴，就是枕叶区的P3,P4,O1,O2数据
ch_size=length(ch);
data_index=0;
data_unfilter=zeros(DATA_LENGTH,ch_size);
data=zeros(DATA_LENGTH,ch_size);

freq = [10,9,7.5,11.25,6.4];
frecount = length(freq);
peroid=1000./freq;
segment=Fs;%窗长
slide=Fs/8;%滑动窗长

reference = cell(frecount,1);
for i=1:frecount
    reference{i} = [cos(2*pi*freq(i)/Fs*(1:segment));-sin(2*pi*freq(i)/Fs*(1:segment));cos(4*pi*freq(i)/Fs*(1:segment));-sin(4*pi*freq(i)/Fs*(1:segment))];%基波
%    reference{i} = reference{i}';
end
signal=-1;
velocity=0;
variance=0;
output = [signal velocity variance];
sx = 4;%与reference{i}的行维数一致
sy = 4;%与所选取的解算通道数个数一致，即==ch_size
rou = zeros(1,4);