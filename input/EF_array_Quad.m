function [] = EF_array_Quad()
fid = fopen('EF1.txt','r');
temp = fscanf(fid,'%g');
status = fclose(fid);
arr1(1,:) = temp(1:length(temp)/2);
arr1(2,:) = temp(length(temp)/2+1:end);

fid = fopen('EF2.txt','r');
temp = fscanf(fid,'%g');
status = fclose(fid);
arr2(1,:) = temp(1:length(temp)/2);
arr2(2,:) = temp(length(temp)/2+1:end);

fid = fopen('EF3.txt','r');
temp = fscanf(fid,'%g');
status = fclose(fid);
arr3(1,:) = temp(1:length(temp)/2);
arr3(2,:) = temp(length(temp)/2+1:end);

fid = fopen('EF4.txt','r');
temp = fscanf(fid,'%g');
status = fclose(fid);
arr4(1,:) = temp(1:length(temp)/2);
arr4(2,:) = temp(length(temp)/2+1:end);


plot(arr1(1,:),arr1(2,:),arr2(1,:),arr2(2,:),...
    arr3(1,:),arr3(2,:),arr4(1,:),arr4(2,:));
legend( 'Pos drift', 'Pos drift, with options', 'Neg drift',...
    'Neg drift, with options', 'location', 'NorthWest' )