# 特殊模式的设计：
## 细胞分类：
### 生产者 2
### 消费者 3
### 障碍4
## 导入地图中的1会被统一转化为2
 消费者可以选择移动，需要食物生存。不会因为孤独而死，食物不足会移动并杀死生产者
 生产者不需要食物，会为周边8格随机提供食物（每x回合生产1个）
 生产者受到孤独和拥挤影响,但不受寿命影响
 但是当生产者数量过少时将会取消孤独机制，并随机生成新的生产者

# 地图特性
## 灾难模式
 1:change state temperature
 2.randomly kill cells
## 环境变化
 state:acid/alkaline/moderate(PH)
 temperature:High Low Middle
 toxic:on/off


# 逻辑实现：
 1.二维数组记录食物的数量
 2.记录经过的轮数
 3.

* 1

