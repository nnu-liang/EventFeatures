import numpy as np
import matplotlib.pyplot as plt

plt.rcParams.update({'font.size': 40})

# 定义文件列表
files = ["thresholds_errors_acc-mini-epoch27.txt", 
         "thresholds_errors_acc-mini-epoch27-kappa5.txt"]

# 定义颜色和标记样式
colors = ['blue', 'red']  # 每个文件的相对误差曲线颜色
markers = ['o', 'x']      # 每个文件的相对误差标记样式
line_styles = ['-', '--'] # 每个文件的信号效率曲线样式
labels = [r'$\kappa_{\lambda}=1$', r'$\kappa_{\lambda}=5$']  # 图例标签

fig, ax1 = plt.subplots(figsize=(8, 8))

# 主轴1：相对误差
ax2 = ax1.twinx()  # 右侧纵轴1：信号效率
ax3 = ax1.twinx()  # 右侧纵轴2：2B2JPredictHH

# 偏移 ax3 使其不与 ax2 重叠
ax3.spines["right"].set_position(("outward", 100))  # 右移 60 点
fig.subplots_adjust(left=0.1, right=0.85, top=0.9, bottom=0.2)
# 遍历文件，读取数据并绘图
for i, file in enumerate(files):
    data = np.loadtxt(file, skiprows=1)
    thresholds = data[:, 0]
    errors = data[:, 1] * 100  
    accuracies = data[:, 2] * 100 

    # 仅在第一个文件中读取 2B2JPredictHH
    if i == 0:
        twoB2JPredictHH = data[:, 3] * 100  # 读取第四列数据并转换为百分比

    # 找到最小误差的位置
    min_error_index = np.argmin(errors)

    # 数据抽样
    processed_thresholds = np.concatenate((thresholds[:min_error_index:10], thresholds[min_error_index::10]))
    processed_errors = np.concatenate((errors[:min_error_index:10], errors[min_error_index::10]))
    processed_accuracies = np.concatenate((accuracies[:min_error_index:10], accuracies[min_error_index::10]))
    if i == 0:
        processed_twoB2JPredictHH = np.concatenate((twoB2JPredictHH[:min_error_index:10], twoB2JPredictHH[min_error_index::10]))

    # 绘制相对误差曲线
    ax1.plot(processed_thresholds, processed_errors, marker=markers[i], linestyle=line_styles[i], 
             color='blue', label=f'{labels[i]} - Relative Error (%)')

    # 绘制信号效率曲线（右轴1）
    ax2.plot(processed_thresholds, processed_accuracies, marker=markers[i], linestyle=line_styles[i], 
             color='green', label=f'{labels[i]} - Signal Efficiency (%)')

    # 仅对第一个文件绘制 2B2JPredictHH 曲线（右轴2）
    if i == 0:
        ax3.plot(processed_thresholds, processed_twoB2JPredictHH, marker=markers[i], linestyle=line_styles[i], 
                 color='orange', label=r'$2b2j$ mispredicted as $HH$ (%)')

# 设置主轴属性（相对误差）
ax1.set_xlabel('Threshold') 
ax1.set_ylabel('Relative Error (%)', color='blue')  
ax1.tick_params(axis='y', labelcolor='blue')  
ax1.grid()

# 设置次轴1属性（信号效率）
ax2.set_ylabel('Signal Efficiency (%)', color='green')  
ax2.tick_params(axis='y', labelcolor='green')  

# 设置次轴2属性（2B2JPredictHH）
#ax3.set_ylabel('2b2j mispredicted as HH (%)', color='orange')  
ax3.set_ylabel(r'$2b2j$ mispredicted as $HH$ (%)', color='orange')
ax3.tick_params(axis='y', labelcolor='orange')

# 添加辅助线
for x in [0.5, 0.7, 0.9]:
    ax1.axvline(x=x, color='purple', linestyle='--', linewidth=3)

# 合并图例
lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
lines3, labels3 = ax3.get_legend_handles_labels()
combined_lines = lines1 + lines2 + lines3
combined_labels = labels1 + labels2 + labels3
combined_lines.append(plt.Line2D([0], [0], color='none')) 
combined_labels.append(r"$L=3000 \, \mathrm{fb}^{-1}, \, 68\% \, \mathrm{CL}$")
ax1.legend(combined_lines, combined_labels, loc='lower left', bbox_to_anchor=(0, 0.1), fontsize=25)

# 设置标题
#plt.title('Relative Error, Signal Efficiency')

# 显示图表
plt.show()

