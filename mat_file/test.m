%����
%for j = 1:1264
if(signal>=0&&signal<4&&IS_TRAIN==0)
    mark(signal+1) = mark(signal+1)+positive;
    for i=1:4
        if(mark(i) >= negative)
            mark(i) = mark(i) - negative;
        end
    end
    for i=1:4
        if(mark(i)>=threshold)
            serialindex = index;%serialindex+1;
            mark(i) = mark(i) - minus;
            serial(serialindex) = i-1;
            serial_tri(serialindex) = trigger(index);
%            seriall = [seriall i-1];
 %           serial_trii = [serial_trii trigger(index)];
        end
    end
end
%end
% for j = 1:1264
% if(Signal(j)>=0&&Signal(j)<4&&IS_TRAIN==0)
%     mark(Signal(j)+1) = mark(Signal(j)+1)+positive;
%     for i=1:4
%         if(mark(i) >= negative)
%             mark(i) = mark(i) - negative;
%         end
%     end
%     for i=1:4
%         if(mark(i)>=threshold)
%             mark(i) = mark(i) - minus;
%             serial = [serial i-1];
%         end
%     end
% end
% end
% dataa = [];
% num = 0;
% for i = 1:32
%     indices = 0;
%     indices = find(data(i,:)==0 | data(i,:)==1 | data(i,:)==2 | data(i,:)==3);
%     dataa = [dataa data(i,indices)];
%     num = num+length(indices);
% end