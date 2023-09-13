// #include <stdio.h>

// // Function to find the median
// double findMedian(int arr[], int size) {
//     int min_val = arr[0];
//     int max_val = arr[0];
//     int count_less = 0;
//     int count_greater = 0;

//     // Find the minimum and maximum values
//     for (int i = 1; i < size; ++i) {
//         if (arr[i] < min_val) {
//             min_val = arr[i];
//         }
//         if (arr[i] > max_val) {
//             max_val = arr[i];
//         }
//     }

//     // Count elements less than max_val and greater than min_val
//     for (int i = 0; i < size; ++i) {
//         if (arr[i] < max_val) {
//             count_less++;
//         }
//         if (arr[i] > min_val) {
//             count_greater++;
//         }
//     }

//     // Find the median
//     if (size % 2 == 1) {
//         // Odd number of elements, median is the middle element
//         return arr[(size - 1) / 2];
//     } else {
//         // Even number of elements, median is the average of two middle elements
//         double middle_sum = 0;
//         for (int i = 0; i < size; ++i) {
//             if (arr[i] > min_val && arr[i] < max_val) {
//                 middle_sum += arr[i];
//             }
//         }
//         return middle_sum / count_less;  // Divide by count_less since all middle elements are less than max_val
//     }
// }

// int main() {
//     int data[] = {7, 3, 1, 9, 5};
//     int size = sizeof(data) / sizeof(data[0]);

//     double median = findMedian(data, size);
//     printf("The median is: %.2f\n", median);

//     return 0;
// }


// // V2:
// #include <stdio.h>

// // Function to swap two elements
// void swap(int *a, int *b) {
//     int temp = *a;
//     *a = *b;
//     *b = temp;
// }

// // Function to partition the array and return the pivot index
// int partition(int arr[], int low, int high) {
//     int pivot = arr[high];
//     int i = (low - 1);

//     for (int j = low; j <= high - 1; ++j) {
//         if (arr[j] < pivot) {
//             ++i;
//             swap(&arr[i], &arr[j]);
//         }
//     }

//     swap(&arr[i + 1], &arr[high]);
//     return (i + 1);
// }

// // Function to perform the Quickselect algorithm
// int quickSelect(int arr[], int low, int high, int k) {
//     if (low <= high) {
//         int pivotIndex = partition(arr, low, high);

//         if (pivotIndex == k) {
//             return arr[pivotIndex];
//         }

//         if (pivotIndex < k) {
//             return quickSelect(arr, pivotIndex + 1, high, k);
//         } else {
//             return quickSelect(arr, low, pivotIndex - 1, k);
//         }
//     }

//     // This return statement should not be reached, but it's included to avoid a compilation warning.
//     return -1;
// }

// // Function to find the median
// double findMedian(int arr[], int size) {
//     if (size % 2 == 1) {
//         // For odd-sized array, the median is at index (size - 1) / 2
//         return quickSelect(arr, 0, size - 1, (size - 1) / 2);
//     } else {
//         // For even-sized array, the median is the average of elements at indexes size / 2 - 1 and size / 2
//         int leftMedian = quickSelect(arr, 0, size - 1, size / 2 - 1);
//         int rightMedian = quickSelect(arr, 0, size - 1, size / 2);
//         return (leftMedian + rightMedian) / 2.0;
//     }
// }

// int main() {
//     int data[] = {7, 3, 1, 9, 5};
//     int size = sizeof(data) / sizeof(data[0]);

//     double median = findMedian(data, size);
//     printf("The median is: %.2f\n", median);

//     printf("Array:\n");
//     for (int i = 0; i < size; i++) {
//         printf("%d ", data[i]);
//     }
//     printf("\n");

//     return 0;
// }