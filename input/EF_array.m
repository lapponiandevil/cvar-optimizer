function [] = EF_plot(array)
fid = fopen('EF.txt','r');
temp = fscanf(fid,'%g');
status = fclose(fid);
arr1(1,:) = temp(1:length(temp)/2);
arr1(2,:) = temp(length(temp)/2+1:end);

plot(array(1,:),array(2,:))