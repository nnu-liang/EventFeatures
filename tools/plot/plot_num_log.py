import matplotlib.pyplot as plt
import numpy as np

# 文件名
input_file = "/home/liang/Workingspace/EvenTF-code/thresholds_predictions_real_events.txt"

# 初始化数据结构
thresholds = []
predicted_counts = {i: [] for i in range(9)}  # 每个标签对应的预测数量

# 标签名称按照指定顺序替换
label_names = [
    "hh_4b",
    "tt_2b4j",
    "tth_4b4j",
    "ttbb_4b4j",
    "hbb_4b",
    "4b",
    "2b2j",
    "zz_4b",
    "zh_4b"
]

# 读取数据
with open(input_file, "r") as file:
    lines = file.readlines()
    # 跳过标题行
    for line in lines[1:]:
        parts = line.strip().split()
        threshold = float(parts[0])
        label = int(parts[1])
        count = int(parts[2])  # 预测数量

        # 添加数据
        if label == 0 and threshold not in thresholds:
            thresholds.append(threshold)
        predicted_counts[label].append(count)

# 获取threshold=0.5对应的索引
start_index = thresholds.index(0)
thresholds = thresholds[start_index:]  # 截取从threshold=0.5开始的部分
for label in range(9):
    predicted_counts[label] = predicted_counts[label][start_index:]  # 同样截取

# 设置字体大小
font_size = 25  # 全局字体大小
legend_font_size = 18  # 图例字体大小
plt.rc('font', size=font_size)        # 设置全局字体大小
plt.rc('axes', titlesize=font_size)  # 坐标轴标题字体大小
plt.rc('axes', labelsize=font_size)  # 坐标轴标签字体大小
plt.rc('xtick', labelsize=font_size) # x轴刻度字体大小
plt.rc('ytick', labelsize=font_size) # y轴刻度字体大小
plt.rc('figure', titlesize=font_size) # 图标题字体大小

# 绘制所有标签的曲线（纵轴取 log）
plt.figure(figsize=(14, 10))
handles = []
for label in range(9):  # 包含所有标签
    line, = plt.plot(thresholds, predicted_counts[label], label=label_names[label])  # 绘制曲线
    handles.append(line)  # 将绘制的曲线句柄添加到 handles

# 设置纵轴为 log scale
plt.yscale("log")

# 调整图例
plt.legend(
    handles=handles,
    title="Labels",
    loc="upper center",  # 将图例放置在图外
    fontsize=legend_font_size,
    ncol=3,  # 设置图例分为3列以减少垂直空间占用
    bbox_to_anchor=(0.5, -0.15),  # 图例在图外的相对位置
    frameon=True
)
plt.xlabel("Threshold")
plt.ylabel("Predicted Counts (Log Scale)")
plt.grid(True, linestyle="--", linewidth=0.5)  # 仅启用主要网格
plt.tight_layout()
plt.savefig("predicted_counts_all_labels_log_no_minor_grid.png", bbox_inches="tight")  # 确保图例不被裁剪
plt.show()

