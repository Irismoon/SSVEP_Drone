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
       % if mod(data_index,TRIAL)>(TRIAL_TRAISIENT) || mod(data_index,TRIAL)==0
         if(data_index_mod>segment)
            index=index+1;
            Y=data(data_index_mod-segment+1:data_index_mod,:)';%4��ͨ������������
            for j=1:frecount
                z = [reference{j};Y];%����������ϳ�һ������
                C = cov(z.');      %������Э�������
                Cxx = C(1:sx, 1:sx) + 10^(-8)*eye(sx);   %���X����Э�������
                Cxy = C(1:sx, sx+1:sx+sy);         %���X�ͣٵĻ�Э�������
                Cyx = Cxy';
                Cyy = C(sx+1:sx+sy, sx+1:sx+sy) + 10^(-8)*eye(sy);%���Y����Э�������
                [Wx,r] = eig((Cxx\Cxy)*(Cyy\Cyx));%ע������ţ�r is eigenvalues;A*Wx = r*Wx��r��index��Wx���е�index��Ӧ
                r = sqrt(real(r));      % Canonical correlations   �������ϵ��
                r = diag(r);%����ת�������������·�ת
                [r,I]= sort((real(r)));
                r = flipud(r);	
                rou(j) = r(1);
            end
%               rou(1) = rou(1)+ 0.12;rou(2) = rou(2)+0.02;%LRJ
%              rou(3) = rou(3)-0.1;%WM
%               rou(2) = rou(2)+
%               rou(4) = rou(4)+ 0.07;%WM
 %             rou(2) = rou(2)+0.02; rou(3)=rou(3)+0.05;                   %lss
            %rou(2) = rou(2) - 0.03;%cc
%             rou(1) = rou(1) + 0.08;%cc
%             rou(1) = rou(1)+0.1;
%             rou(2) = rou(2) - 0.03;
%             rou(3) = rou(3)+0.05;
%             rou(4) = rou(4) + 0.09;%YAD
%            R(index,:) = rou;
            [velocity,signal] = max(rou);
            if velocity<=0.42 && velocity>=0.4%yad:0.39%cc:0.43
                signal = 0;
            else
                if velocity < 0.4
                    signal = 5;
                end
            end
         %end
        end  
else
    signal=-1;
    velocity=0;
    variance=0;
    output = [signal velocity variance];
end