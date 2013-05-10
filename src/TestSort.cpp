#include <iostream>
#include <gtest/gtest.h>

#define CHECK_SEQUENCE {\
    EXPECT_EQ(1, _num[0]);  \
    EXPECT_EQ(2, _num[1]);  \
    EXPECT_EQ(3, _num[2]);  \
    EXPECT_EQ(5, _num[3]);  \
    EXPECT_EQ(17, _num[4]); \
    EXPECT_EQ(18, _num[5]); \
    EXPECT_EQ(21, _num[6]); \
    EXPECT_EQ(34, _num[7]); \
}
bool BubbleSort(int *nums, int length);
bool InsertSort(int *nums, int length);
bool ShellSort(int *nums, int length);
void QuickSort(int *nums, int low, int high);
void SelectSort(int *nums, int length);
void HeapSort(int *nums, int length);

static const int num[8] = {2, 34, 1, 3, 5, 21, 18, 17};

class TestSort : public testing::Test {
protected:
    virtual void SetUp() {
        memcpy(_num, num, sizeof(_num));
    }

    virtual void TearDown() {
    }

protected:
    int _num[8];
};

TEST_F(TestSort, testBubbleSort) {
    BubbleSort(_num, 8);
    CHECK_SEQUENCE
}

TEST_F(TestSort, testInsertSort) {
    InsertSort(_num, 8);
    CHECK_SEQUENCE
}

TEST_F(TestSort, testShellSort) {
    ShellSort(_num, 8);
    CHECK_SEQUENCE
}

TEST_F(TestSort, testQuickSort) {
    QuickSort(_num, 0, 7);
    CHECK_SEQUENCE
}

TEST_F(TestSort, testSelectSort) {
    SelectSort(_num, 8);
    CHECK_SEQUENCE
}

TEST_F(TestSort, testHeapSort) {
    HeapSort(_num, 8);
    CHECK_SEQUENCE
}

bool BubbleSort(int *nums, int length) {
    if (NULL == nums)
        return false;

    for (int i=0; i<length-1; i++) {
        bool isExg = false;
        for (int j=i+1; j<length; j++) {
            if (nums[i] > nums[j]) { // change value
                int tmp = nums[j];
                nums[j] = nums[i];
                nums[i] = tmp;
            }
            isExg = true;
        }
        if (!isExg)
            return true;
    }

    return true;
}

bool InsertSort(int *nums, int length) {
    if (NULL == nums) {
        return false;
    }

    for (int i=1; i<length; i++) {
        for (int j=0; j<i; j++) {
            if (nums[i] < nums[j]) {
                int tmp = nums[i];
                for (int n=i; n>j; n--)
                    nums[n] = nums[n-1];
                nums[j] = tmp;
            }
        }
    }

    return true;
}

void _shellPass(int *nums, int d, int length) {
    for (int i=d; i<length; i++) {
        if (nums[i] < nums[i-d]) {
            int tmp = nums[i];
            int j = i - d;
            do {
                nums[i] = nums[j];
                j -= d;
            } while (j>=0 && tmp<nums[j]);
            nums[j+d] = tmp;
        }
    }
}

bool ShellSort(int *nums, int length) {
    if (NULL == nums) {
        return false;
    }

    int d = length;
    while (d > 1) {
        d = d / 3 + 1;
        _shellPass(nums, d, length);
    }

    return true;
}

int _partition(int *nums, int low, int high) {
    int pivot = nums[low];
    int i = low + 1;
    int j = high;
    while (true) {
        while ((nums[i] <= pivot) && (i <= j)) {
            i++;
        }
        while ((nums[j] >= pivot) && (i <= j)) {
            j--;
        }
        if (i >= j) {
            break;
        }
        int tmp = nums[i];
        nums[i] = nums[j];
        nums[j] = tmp;
    }
    nums[low] = nums[j];
    nums[j] = pivot;

    return j;
}

void QuickSort(int *nums, int low, int high) {
    if (low < high) {
        int pivotPos = _partition(nums, low, high);
        QuickSort(nums, low, pivotPos-1);
        QuickSort(nums, pivotPos+1, high);
    }
}

void SelectSort(int *nums, int length) {
    int k = 0;
    for (int i=0; i<length-1; i++) {
        k = i;
        for (int j=i+1; j<length; j++) {
            if (nums[k] > nums[j]) {
                k = j;
            }
        }
        int tmp = nums[i];
        nums[i] = nums[k];
        nums[k] = tmp;
    }
}

void Heapify(int *nums, int low, int high) {
    int i = low;
    int j = low * 2;
    while (j <= high) {
        if ((j+1) <= high) {
            if (nums[j] < nums[j+1]) {
                j++;
            }
        }
        if (nums[i] < nums[j]) {
            int tmp = nums[i];
            nums[i] = nums[j];
            nums[j] = tmp;
        }
        i *= 2;
        j = i * 2;
    }
}

void BuildHeap(int *nums, int length) {
    for (int i=length/2; i>0; i--) {
        Heapify(nums, i, length);
    }
}

void HeapSort(int *nums, int length) {
    int tmpNums[length+1];
    for (int k=1; k<=length; k++) {
        tmpNums[k] = nums[k-1];
    }
    BuildHeap(tmpNums, length);
    for (int i=length; i>=2; i--) {
        int tmp = tmpNums[i];
        tmpNums[i] = tmpNums[1];
        tmpNums[1] = tmp;
        Heapify(tmpNums, 1, i-1);
    }

    for (int k=1; k<=length; k++) {
        nums[k-1] = tmpNums[k];
    }
}

