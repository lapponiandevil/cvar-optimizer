function [] = PL_plot(bins)
fid = fopen('PL1.dat','r');
PL1 = fscanf(fid,'%g');
status = fclose(fid);
PL1 = sort(PL1);
PL1 = PL1(100:end-100);


fid = fopen('PL2.dat','r');
PL2 = fscanf(fid,'%g');
status = fclose(fid);
PL2 = sort(PL2);
PL2 = PL2(100:end-100);


fid = fopen('PL3.dat','r');
PL3 = fscanf(fid,'%g');
status = fclose(fid);
PL3 = sort(PL3);
PL3 = PL3(100:end-100);


fid = fopen('PL4.dat','r');
PL4 = fscanf(fid,'%g');
status = fclose(fid);
PL4 = sort(PL4);
PL4 = PL4(100:end-100);

subplot(2,2,1), hist(PL1,bins)
subplot(2,2,2), hist(PL2,bins)
subplot(2,2,3), hist(PL3,bins)
subplot(2,2,4), hist(PL4,bins)
