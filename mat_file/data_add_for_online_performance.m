data_index=data_index+1;
data_index_mod=mod(data_index-1,DATA_LENGTH)+1;
%在线滤波
data_unfilter(data_index_mod,:)=x(ch);%C++里扔进来的数据存起来
if data_index_mod<=filt_n+1
    xx=[filter_b(2:data_index_mod),-filter_a(2:data_index_mod)];
    yy=[data_unfilter(data_index_mod-1:-1:1,:);data(data_index-1:-1:1,:)];
else
    xx=[filter_b(2:filt_n+1),-filter_a(2:filt_n+1)];
    yy=[data_unfilter(data_index_mod-1:-1:data_index_mod-filt_n,:);
        data(data_index_mod-1:-1:data_index_mod-filt_n,:)];
end
data(data_index_mod,:)=filter_b(1)*data_unfilter(data_index_mod,:);
if(size(xx,2)>0)
    data(data_index_mod,:)=data(data_index_mod,:)+xx*yy;
end

%数据处理
if mod(data_index,slide)==0%每slide个数据处理一次
        if mod(data_index,TRIAL)>(TRIAL_TRAISIENT) || mod(data_index,TRIAL)==0
         if(data_index_mod>segment)
            index=index+1;
            Y=data(data_index_mod-segment+1:data_index_mod,:)';%4个通道窗长内数据
            for i=1:frecount
                z = [reference{i};Y];%将两个矩阵合成一个矩阵
                C = cov(z.');      %计算自协方差矩阵
                Cxx = C(1:sx, 1:sx) + 10^(-8)*eye(sx);   %求得X的自协方差矩阵
                Cxy = C(1:sx, sx+1:sx+sy);         %求得X和Ｙ的互协方差矩阵
                Cyx = Cxy';
                Cyy = C(sx+1:sx+sy, sx+1:sx+sy) + 10^(-8)*eye(sy);%求得Y的自协方差矩阵
                [Wx,r] = eig(inv(Cxx)*Cxy*inv(Cyy)*Cyx);%r is eigenvalues;A*Wx = r*Wx，r的index与Wx的列的index对应
                r = sqrt(real(r));      % Canonical correlations   典型相关系数
                r = diag(r);%矩阵转换成向量后上下翻转
%                 V = fliplr(Wx);%左右翻转
                [r,I]= sort((real(r)));
                r = flipud(r);	
%                 for j = 1:length(I)
%                     Wx(:,j) = V(:,I(j));  % sort reversed eigenvectors in ascending order
%                 end
%                 Wx = fliplr(Wx);
                rou(i) = r(1);
            end
            R(index,:) = rou;
            [velocity,signal] = max(rou);
            variance = var(rou);
            signal = signal-1;
            output = [signal velocity variance];
            Signal(index) = signal;
            trigger(index) = x(DATA_CHANNEL+1);
         end
        end  
else
    signal=-1;
    velocity=0;
    variance=0;
    output = [signal velocity variance];
end