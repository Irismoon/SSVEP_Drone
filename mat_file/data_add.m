data_index=data_index+1;
data_index_mod=mod(data_index-1,DATA_LENGTH)+1;
%�����˲�
data_unfilter(data_index_mod,:)=x(ch);%C++���ӽ��������ݴ�����
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

%���ݴ���
if mod(data_index,slide)==0%ÿslide�����ݴ���һ��
         if(data_index_mod>segment)
            Y=data(data_index_mod-segment+1:data_index_mod,:)';%4��ͨ������������
            for i=1:frecount
                z = [reference{i};Y];%����������ϳ�һ������
                C = cov(z.');      %����8x8��Э�������zת�õ�ÿһ�д���һ����������������Э�����8��������4��ͨ��+4���ο���
                Cxx = C(1:sx, 1:sx) + 10^(-8)*eye(sx);   %���reference��������Э�������
                Cxy = C(1:sx, sx+1:sx+sy);         %���X�ͣٵĻ�Э�������
                Cyx = Cxy';
                Cyy = C(sx+1:sx+sy, sx+1:sx+sy) + 10^(-8)*eye(sy);%���Y����Э�������
                [Wx,r] = eig(Cxx\Cxy*(Cyy\Cyx));%r is eigenvalues;A*Wx = r*Wx��r��index��Wx���е�index��Ӧ
                r = sqrt(real(r));      % Canonical correlations   �������ϵ��
                r = diag(r);%����ת�������������·�ת
%                 V = fliplr(Wx);%���ҷ�ת
                [r,I]= sort((real(r)));
                r = flipud(r);	
%                 for j = 1:length(I)
%                     Wx(:,j) = V(:,I(j));  % sort reversed eigenvectors in ascending order
%                 end
%                 Wx = fliplr(Wx);
                rou(i) = r(1);
            end
            [velocity,signal] = max(rou);
            variance = var(rou);
            output = [signal velocity variance];
         end
else
    signal=-1;
    velocity=0;
    variance=0;
    output = [signal velocity variance];
end