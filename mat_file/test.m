%≤‚ ‘
a(:,1) = C;
a(:,2) = trigger(1:N/4);
a(:,3) = a(:,2) - a(:,1);
line(1:length(a(:,1)),a(:,2));hold on;
plot(1:length(a(:,1)),a(:,3),'r*');