tic;head_for_offline_performance;
mark = zeros(1,4);
threshold = 30;
negative = 10;
positive = 20;
minus = 10;
IS_TRAIN=0;
R = zeros(1280,4);
data_src=load('data_cnt-2016.05.10_12.16.txt');
for i=1:DATA_LENGTH
    x=data_src(i,:);
    data_add_for_offline_performance;
    test;
end
toc;
 N=length(trigger);
% [C,err,P,logp,coeff] = classify(fea_tran(1:N/2,:),fea_tran(1+N/2:N,:),trigger(1+N/2:N));%交叉验证
 disp(sum((Signal-trigger(1:N))==0)/N*100);
plot(1:N,trigger);hold on;scatter(1:N,Signal);%分类器实时分类结果
figure(2)
plot(1:N,trigger);hold on;scatter(1:length(serial),serial);axis([0 1280 0 3])%经过控制策略控制后的输出结果