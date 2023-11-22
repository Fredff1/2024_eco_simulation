#include <stdio.h>
void merge(int a[], int aux[], int low, int mid, int high) {
    // 用aux暂时存放a的数据
    // 然后修改a的内容
    for (int k = low; k <= high; k += 1) {
        aux[k] = a[k];
    }
    int i = low, j = mid + 1;
    for (int k = low; k <= high; k += 1) {
        // TODO
        // 注意:
        // 当某一边的子数组先归并完之后应该怎样处理
        // 仔细思考应该如何移动光标i和j

        //合并的方法
        if(i>mid){
            a[k]=aux[j];
            j++;
        }else if(j>high){
            a[k]=aux[i];
            i++;
            //这两个是为了确保在一个子数组用完了后全部复制到另一个数组
        }else if(aux[i]<aux[j]){
            a[k]=aux[i];
            i++;
        }else {
            a[k]=aux[j];
            j++;
        }
        //双指针法，即把两个子数组中较小的一个复制到原来的数组中，然后将其后移一位
    }
}

// 将a[low], ..., a[high]这些元素排序好
void sort_impl(int a[], int aux[], int low, int high) {
    if(high<=low){
        return;
    }
    int mid=(high+low)/2;
    sort_impl(a,aux,low,mid);
    sort_impl(a,aux,mid+1,high);
    merge(a,aux,low,mid,high);
    // TODO
    // 什么时候不用继续递归？
}
void sort(int a[], int length) {
    int aux[length];
    sort_impl(a,aux,0,length-1);
    // TODO: 接下来只需要一行代码
}

int main() {
    int a[] = {7, 4, 1, 8, 5, 2, 0, 9, 6, 3};
    sort(a, 10);
    for (int i = 0; i < 10; i += 1) {
        printf("%d ", a[i]);
    }
    printf("\n");
}