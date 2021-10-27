% clear all
% %clear mypi;
% %r=raspi('192.168.1.33','pi','raspberry');
% %mypi=raspi('192.168.1.33','pi','raspberry');
% mypi=raspi('169.254.111.33','pi','raspberry');
% %mypi=raspi;
% %myserialdevice = serialdev(mypi,'/dev/serial0',115200);
% myserialdevice = serialdev(mypi,'/dev/ttyAMA1',115200);
% myserialdevice.Timeout = 0.1;
% n=200;
% 
% 
% while 1
%    
%     serialData = read(myserialdevice,n,'char')
%     flag1=0;flag2=0;
%     count=0;
%     n_ser=0;
% %     while flag1==0
% %         count=count+1;
% %         ini=serialData(1,count);
% %         if ini=='A'
% %             flag1=1;
% %         end
% %     end
% %     
% %     while flag2==0
% %         count=count+1;
% %         fin=serialData(1,count);
% %         if fin~='B'
% %             n_ser=n_ser+1;
% %             cadena(1,n_ser)=fin;
% %         else
% %             flag2=1;
% %         end
% %         
% %     end
% %     
% %     x= strsplit(cadena,';');
% %     x1=string(x(1,1));
% %     x2=string(x(1,2));
% %     y1=strsplit(x1,'#');
% %     y2=strsplit(x2,'#');;
% %     
% %     lect1=str2num(y1(1,2));
% %     lect2=str2num(y2(1,2));
% %     
% %     num=str2num(y1(1,1));
% %     num=uint8(num/10);
% %     
% %     if num==1
% %        x
% %        mod1_lect(1,1)= lect1;
% %        mod1_lect(1,2)= lect2;
% %     end
% %     
% %     if num==2
% %        mod2_lect(1,1)= lect1;
% %        mod2_lect(1,2)= lect2;
% %     end
% %     
% %     if num==3
% %        mod3_lect(1,1)= lect1;
% %        mod3_lect(1,2)= lect2;
% %     end
% %     
% %     if num==4
% %        mod4_lect(1,1)= lect1;
% %        mod4_lect(1,2)= lect2;
% %     end
% %     
% %     if num==5
% %        mod5_lect(1,1)= lect1;
% %        mod5_lect(1,2)= lect2;
% %     end
% %     
% %     if num==6
% %        mod6_lect(1,1)= lect1;
% %        mod6_lect(1,2)= lect2;
% %     end
% %     
% %     if num==7
% %        mod7_lect(1,1)= lect1;
% %        mod7_lect(1,2)= lect2;
% %     end
% %     
% %     if num==8
% %        mod8_lect(1,1)= lect1;
% %        mod8_lect(1,2)= lect2;
% %     end
% %     %cadena
% %      clearvars cadena
%     
% end



clear all
%clear mypi;
%r=raspi('192.168.1.33','pi','raspberry');
%mypi=raspi('192.168.1.33','pi','raspberry');
mypi=raspi('169.254.111.33','pi','raspberry');
%mypi=raspi;
%myserialdevice = serialdev(mypi,'/dev/serial0',115200);
myserialdevice = serialdev(mypi,'/dev/ttyAMA1',115200);
myserialdevice.Timeout = 0.5;
n=600;

num=16;
lecturas=zeros(1,num);
z=zeros(num,2);
flag1=0;flag2=0;
count=0;coma=0;
n_ser=0;

while 1
    serialData = read(myserialdevice,n,'char')
    size(serialData)
end
%     flag1=0;flag2=0;
%     count=0;coma=0;
%     n_ser=0;
% 
% while flag1==0
%     count=count+1;
%     ini=serialData(1,count);
%     if ini==';'
%         flag1=1;
%     end
% end
% 
% while flag2==0
%     count=count+1;
%     fin=serialData(count);
%     
%     if fin==';'
%         coma=coma+1;
%     end
%     
%     if coma<16
%         n_ser=n_ser+1;
%         cadena(1,n_ser)=fin;
%     else
%         flag2=1;
%     end
%     
% end
% 
% x= strsplit(cadena,';');
% for j=1:num
%         y(j,1)=string(x(1,j));
% end
% 
% for j=1:num
%         z(j,:)=strsplit(y(j,1),'#');
% end
% 
% for j=1:num
%         lecturas(1,z(j,1))=z(j,2);
% end
% 
% lecturas(1,1)
% 
% end
