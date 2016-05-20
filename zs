#!/bin/bash
# Author : buwailee
# Script follows here:
a[1]="A：冬马还是雪菜？"
a[2]="B：冬马！"
a[3]="A：你这是在找死，再说一次我就要rm -rf /咯！"
a[4]="A：怎么不说话了？玩你的啦(￢_￢)ﾉ"
a[5]="A：别这样了（严肃脸）。"
a[6]="A：为什么会变成这样呢？"
a[7]="#一个悲伤的故事#"
a[8]=""

n=0
for line in `cat ~/.count`
	do
		((n++));
	done

echo ${a[${n}]}

if [[ $n == 8 ]]; then
	echo $n > ~/.count;
else
	echo $n >> ~/.count;
fi