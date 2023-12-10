import os
import math
import numpy as np
N = 1 # 连续运行 zj0.root 到 zj(N-1).root
#produce .root and .dat file
def main():
    
    for i in range(N):  #or (0,17)
        command = "./DelphesHepMC cards/delphes_card_CMS.tcl hh0"+ str(i) +".root hh.hepmc"
 #第一个是DdlphesHepMC的路径，第二个是delphes_card_CMS.tcl的路径，第三个是输出root文件的名称，第四个是输入的hepmc文件
        os.system(command)#在系统的命令行执行command字符串中的命令
        print("hh0"+ str(i)+".root 完毕！")
        file_in = "hh0" + str(i) + ".root"
        command1 = "./Taggin/Tagging " + file_in   #对.root执行Tagging程序，最后输出一个.dat文件     
        os.system(command1)
        #command2 = "rm -r hh0" + str(i) + ".root"
        #os.system(command2) 
        print('分析文件：' + file_in+'完毕')       
    print('dat完毕！')
if __name__ == '__main__':
    main()

    
# #merge .txt file
# def main():
    # file = open("hh0.txt","w")
    # #N = 3
    # for j in range(N):          
        # tts = open("fout_hh0"+str(j)+".root.dat","r") 
        # for tt in tts:
            # for t in tt:
                # file.writelines(t)
        # #file.write('#\n')
    # file.close()
    # #command3 = "rm -r fout_tt"+str(j)+".root.dat"
    # #os.system(command3)
    # print("txt完毕！")
# if __name__ == '__main__':
    # main()


# #produce x.txt file
# oldf = open("hh0.txt","r")
# a1 = 0
# lines = oldf.readlines()
# for k in lines:
	# ls = k.split()
	# if len(ls) == 1:
		# a1 +=1		
# print(a1)    
   
# newf = open('hh0x.txt','w')
# i = 0
# for line in lines:
    # ls = line.split()  #用split将该行的单词分割成列表，每个单词就时一个列表项目
    # if len(ls) == 11:
        # newf.write(ls[0]) #写入pt
        # newf.write("   ")
        # x = float(ls[2])
        # x = round(x,5) #保留小数点后5位
        # y = float(ls[1])
        # y = round(y,5)
        # i = int((x+0.7)/0.035)
        # j = 39 - int((y+0.7)/0.035)
        # #if i == 40:  #把边缘的点去掉
        # #    i = 39
        # newf.write(str(i)) #把eta换成i  将 eta phi坐标轴换成40*40的格子     
        # newf.write("   ")  #空格
        # newf.write(str(j)) #把phi换成j
        # newf.write("   ")
                 
        # newf.write("\r\n") #换行        
    # if len(ls) == 1: 
        # newf.write(ls[0]) #写入第一列的“#”
        # newf.write("\r\n")       
# oldf.close()
# newf.close()

# #make .npy file    
# file = open('hh0x.txt','r')
# lines = file.readlines()
# b1 = 0
# for k in lines:
	# ls = k.split()
	# if len(ls) == 1:
		# b1 +=1		
# print(b1) 
   
# x = np.zeros((b1-1,1,40,40),dtype = np.float32)
# i = 0
# for line in lines:
    # ls = line.split()
    # if len(ls) == 3: 
        # a = int(ls[1])
        # b = int(ls[2])
        
        # x[i,0,a,b] = x[i,0,a,b] +float(ls[0])
        
    # if len(ls) == 1:
        # i = i + 1 
    # if i == b1-1:
        # break
# print(x.shape)
# np.save("hh0x.npy",x)
# file.close()
# print(".npy 完毕！")  


# os.mkdir('hh0')    
# for y in range(N):
    # command4 = "mv fout_hh0"+ str(y) +".root.dat hh0"
    # os.system(command4)
# print('大功告成！')
   
# command5 = "mv hh0.txt s1.0train"
# os.system(command5)
# command6 = "mv hh0x.txt s1.0train"
# os.system(command6)
# #command7 = "mv hh0x.npy s2.0q1"
# #os.system(command7)
# print('移动成功！')   

# #command8 = "python3 s1.401.py"
# #os.system(command8)       
    
   
