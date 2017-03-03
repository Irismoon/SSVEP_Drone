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
    if IS_TRAIN%train
        if mod(data_index,TRIAL)>TRIAL_TRAISIENT || mod(data_index,TRIAL)==0
            index=index+1;
            for i=1:ch_size
                Y=data(data_index_mod-segment+1:data_index_mod,i);%一个通道窗长内数据
                for j = 1:frecount
                    fea_train(index,(i-1)*frecount+j)=norm(tri{j}*Y);
                end
            end%提取特征
            for i = 1:frecount
                fea_tran(index,i) = fea_train(index,i)+fea_train(index,2*frecount+i)+fea_train(index,3*frecount+i)+fea_train(index,frecount+i);%第i或i/2个频率的基波
            end
            trigger(index)=x(DATA_CHANNEL+1);%x的最后一列是标签
            %disp([data_index, trigger(index)])
            if data_index==DATA_LENGTH
                N=length(trigger);
                [C,err,P,logp,coeff] = classify(fea_tran(1,:),fea_tran(1:N,:),trigger(1:N));
                save('coeff.mat','coeff');%不同类别之间的边界曲线的参数
            end%到最后一行数据执行分类
        end  
        signal=-1;
    else%play
        if data_index_mod>segment%
            for i=1:ch_size
                Y=data(data_index_mod-segment+1:data_index_mod,i);
                for j = 1:2*frecount
                    fea(1,(i-1)*2*frecount+j)=norm(tri{j}*Y);
                end
                
            end
            for i = 1:2*frecount
                feaa(1,i) = fea(1,i)+fea(1,2*frecount+i)+fea(1,3*frecount+i)+fea(1,frecount+i);%第i或i/2个频率的基波/谐波
            end
            r=zeros(1,4);
            r(1,1)=r(1,1)+(coeff(1,2).linear'*fea'+coeff(1,2).const>0);%x*L+K>0，边界曲线是一次函数
            r(1,1)=r(1,1)+(coeff(1,3).linear'*fea'+coeff(1,3).const>0);
            r(1,1)=r(1,1)+(coeff(1,4).linear'*fea'+coeff(1,4).const>0);

            r(1,2)=r(1,2)+(coeff(2,1).linear'*fea'+coeff(2,1).const>0);
            r(1,2)=r(1,2)+(coeff(2,3).linear'*fea'+coeff(2,3).const>0);
            r(1,2)=r(1,2)+(coeff(2,4).linear'*fea'+coeff(2,4).const>0);

            r(1,3)=r(1,3)+(coeff(3,1).linear'*fea'+coeff(3,1).const>0);
            r(1,3)=r(1,3)+(coeff(3,2).linear'*fea'+coeff(3,2).const>0);
            r(1,3)=r(1,3)+(coeff(3,4).linear'*fea'+coeff(3,4).const>0);
            
            r(1,4)=r(1,4)+(coeff(4,1).linear'*fea'+coeff(4,1).const>0);
            r(1,4)=r(1,4)+(coeff(4,2).linear'*fea'+coeff(4,2).const>0);
            r(1,4)=r(1,4)+(coeff(4,3).linear'*fea'+coeff(4,3).const>0);

            [b,signal]=max(r(1,1:4));
            signal=signal-1%配合C++语言，结果出来的是0，1，2，3。
            Signal= [Signal signal];
        end
    end
else
    signal=-1;
end


