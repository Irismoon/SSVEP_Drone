tic;head_for_online_performance;
mark = zeros(1,4);
threshold = 30;
negative = 10;
positive = 20;
minus = 10;
IS_TRAIN=0;
%load('data.mat');
% for k=1:3
%     for i=1:30000
%         x=[data_train{k}(i,:),k];
%         data_add;
%     end
% end

%data_src=load('..\\data-2010.07.11_23.20\\data_cnt-2010.07.11_23.20.txt');
data_src=load('data_cnt-2016.05.10_12.16.txt');
%data_src = data_src.data;
for i=1:DATA_LENGTH
    x=data_src(i,:);
    data_add_for_online_performance;
    test;
end
toc;
% N=length(trigger);
% [C,err,P,logp,coeff] = classify(fea_tran(1:N/2,:),fea_tran(1+N/2:N,:),trigger(1+N/2:N));%交叉验证
% disp(sum((C-trigger(1:N/2))==0)/N*2*100);
% r=zeros(N,4);
% for i=1:N/2
%     r(i,1)=r(i,1)+(coeff(1,2).linear'*fea_train(N/2+i,:)'+coeff(1,2).const>0);
%     r(i,1)=r(i,1)+(coeff(1,3).linear'*fea_train(N/2+i,:)'+coeff(1,3).const>0);
%     r(i,1)=r(i,1)+(coeff(1,4).linear'*fea_train(N/2+i,:)'+coeff(1,4).const>0);%0标签与1，2，3标签分别两两判别
%     r(i,2)=r(i,2)+(coeff(2,1).linear'*fea_train(N/2+i,:)'+coeff(2,1).const>0);
%     r(i,2)=r(i,2)+(coeff(2,3).linear'*fea_train(N/2+i,:)'+coeff(2,3).const>0);
%     r(i,2)=r(i,2)+(coeff(2,4).linear'*fea_train(N/2+i,:)'+coeff(2,4).const>0);%1标签与0，2，3标签判别
%     r(i,3)=r(i,3)+(coeff(3,1).linear'*fea_train(N/2+i,:)'+coeff(3,1).const>0);
%     r(i,3)=r(i,3)+(coeff(3,2).linear'*fea_train(N/2+i,:)'+coeff(3,2).const>0);
%     r(i,3)=r(i,3)+(coeff(3,4).linear'*fea_train(N/2+i,:)'+coeff(3,4).const>0);%2与0，1，3判别
%     r(i,4)=r(i,4)+(coeff(4,1).linear'*fea_train(N/2+i,:)'+coeff(4,1).const>0);
%     r(i,4)=r(i,4)+(coeff(4,2).linear'*fea_train(N/2+i,:)'+coeff(4,2).const>0);
%     r(i,4)=r(i,4)+(coeff(4,3).linear'*fea_train(N/2+i,:)'+coeff(4,3).const>0);%3与0,1,2判别
%     r(i,5)=C(i);
%     [b,r(i,6)]=max(r(i,1:4));
% end
% % 
% figure()
% hold on
% for i=1:length(fea_train)
%     if trigger(i)==0
%         sty='b*';
%     end
%     if trigger(i)==1
%         sty='ro';
%     end
%     if trigger(i)==2
%         sty='gp';
%     end
%     if(trigger(i)==3)
%         sty = 'y*';
%     end
%     plot3(coeff(1,2).linear'*fea_train(i,:)'+coeff(1,2).const,...
%         coeff(1,3).linear'*fea_train(i,:)'+coeff(1,3).const,...
%         coeff(1,4).linear'*fea_train(i,:)'+coeff(1,4).const,...
%         coeff(2,3).linear'*fea_train(i,:)'+coeff(2,3).const,...
%         coeff(2,4).linear'*fea_train(i,:)'+coeff(2,4).const,...
%         coeff(3,4).linear'*fea_train(i,:)'+coeff(3,4).const,sty);
% end
% 
% figure(2)
% for i=100:length(fea_test)
%     plot3(coeff(1,2).linear'*fea_test(i,:)'+coeff(1,2).const,...
%         coeff(1,3).linear'*fea_test(i,:)'+coeff(1,3).const,...
%         coeff(2,3).linear'*fea_test(i,:)'+coeff(2,3).const,'k.');
% end
% view(3)
% 
% [C,err,P,logp,coeff] = classify(fea_test,fea_train(1:N,:),trigger(1:N));