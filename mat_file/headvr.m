clc;clear all;
Fs=128;
filt_n=4;
[filter_b,filter_a]=butter(filt_n,2/(Fs/2),'high');%4�װ�����˹��ͨ�˲���

DATA_LENGTH=200*Fs;
DATA_CHANNEL=8;
ch=[1,3,5,7];%ָǰ��AF3,AF4,F3,F4,����Emotiv���Ŵ���������Ҷ����P3,P4,O1,O2����
ch_size=length(ch);
data_index=0;
data_unfilter=zeros(DATA_LENGTH,ch_size);
data=zeros(DATA_LENGTH,ch_size);

freq = [10,9,7.5,11.25,6.4];
frecount = length(freq);
peroid=1000./freq;
segment=Fs;%����
slide=Fs/8;%��������

reference = cell(frecount,1);
for i=1:frecount
    reference{i} = [cos(2*pi*freq(i)/Fs*(1:segment));-sin(2*pi*freq(i)/Fs*(1:segment));cos(4*pi*freq(i)/Fs*(1:segment));-sin(4*pi*freq(i)/Fs*(1:segment))];%����
%    reference{i} = reference{i}';
end
signal=-1;
velocity=0;
variance=0;
output = [signal velocity variance];
sx = 4;%��reference{i}����ά��һ��
sy = 4;%����ѡȡ�Ľ���ͨ��������һ�£���==ch_size
rou = zeros(1,4);