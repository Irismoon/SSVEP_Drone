clc;clear all;
IS_TRAIN=1;
Fs=128;
filt_n=4;
[filter_b,filter_a]=butter(filt_n,2/(Fs/2),'high');%4阶巴特沃斯高通滤波器


DATA_LENGTH=160*Fs;
Signal = [];
DATA_CHANNEL=8;
ch=[1,3,5,7];%指前额AF3,AF4,F3,F4,但是Emotiv反着戴，就是枕叶区的P3,P4,O1,O2数据
ch_size=length(ch);
data_index=0;
index=0;
data_unfilter=zeros(DATA_LENGTH,ch_size);
data=zeros(DATA_LENGTH,ch_size);

freq = [12.525,9.3439,8.2973,7.5125];%LBH
%freq = [12.525,9.4934,8.2973,15.0249];%LSZ
%freq = [12.5 9.37 8.33 6.8182];
%freq=[12 10 8.5714 6.667];
frecount = length(freq);
peroid=1000./freq;
%freq=[12 8.5714 6.6667]
%peroid=[80,120,200];
segment=2*Fs;%窗长
slide=Fs/8;%滑动窗长
TRIAL=Fs*5;%训练时的每个同类标签的长度
TRIAL_TRAISIENT=Fs/4+2*Fs;
TRIAL_STEADY=TRIAL-TRIAL_TRAISIENT;%每个trial，25%的数据是暂态数据

fea_train=zeros(floor(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide),frecount*ch_size);
fea_tran=zeros(floor(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide),frecount);
fea=zeros(1,frecount*ch_size);
feaa=zeros(1,frecount*ch_size);
trigger=zeros(DATA_LENGTH*(TRIAL_STEADY/TRIAL)/slide,1);
tri=cell(frecount,1);
for i=1:frecount
    tri{i}=[cos(2*pi*freq(i)/Fs*(1:segment));-sin(2*pi*freq(i)/Fs*(1:segment))];
    %tri{i+frecount}=[cos(2*pi*2*freq(i)/Fs*(1:segment));-sin(2*pi*2*freq(i)/Fs*(1:segment))];
end
signal=-1;