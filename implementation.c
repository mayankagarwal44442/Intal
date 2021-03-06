#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int max(int a, int b)    //int suffices as atmost length is 1000
{
    return (a>b)?a:b;
}

static int min(int a, int b)    //int suffices as atmost length is 1000
{
    return (a<b)?a:b;
}


static void reverse_string(char* intal)
{
    int n = strlen(intal);
    for(int i = 0; i < n/2; ++i)
    {
        char temp = intal[i];
        intal[i] = intal[n-1-i];
        intal[n-1-i] = temp;
    }
}


static void swap_strings(char** s1, char** s2)
{
    char* temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}


char* intal_add(const char* intal1, const char* intal2)
{
    //we need copies to work with as the input can be a literal too which
    // is unmodifiable
    char* num1 = (char*)malloc((strlen(intal1) + 1)* sizeof(char));
    char* num2 = (char*)malloc((strlen(intal2) + 1)* sizeof(char));
    strcpy(num1, intal1);
    strcpy(num2, intal2);

    int size1 = strlen(num1);
    int size2 = strlen(num2);

    int max_res_size = max(size1, size2) + 1;   
    //maximum sum of two numbers can atmost be one digit more
    char* res = (char*)malloc((max_res_size + 1)*sizeof(char));
    // +1 for the null character

    reverse_string(num1);
    reverse_string(num2);

    int i = 0, j = 0;

    int carry = 0;
    while(num1[i] && num2[j])
    {
        res[i] = ((num1[i] - '0') + (num2[j] - '0') + carry)%10 + '0';
        carry = ((num1[i] - '0') + (num2[j] - '0') + carry)/10;
        ++i;
        ++j;
    }
    if(num1[i])
    {
        while(num1[i])
        {
            res[i] = ((num1[i] - '0')  + carry)%10 + '0';
            carry = ((num1[i] - '0')  + carry)/10;
            ++i;
        }
        if(carry)
        {
            res[i] = carry + '0';
            ++i;
        }
        res[i] = '\0';
    }
    else if(num2[j])
    {
        while(num2[j])
        {
            res[j] = ((num2[j] - '0')  + carry)%10 + '0';
            carry = ((num2[j] - '0')  + carry)/10 ;
            ++j;
        }
        if(carry)
        {
            res[j] = carry + '0';
            ++j;
        }
        res[j] = '\0';
    }
    else
    {
        if(carry)
        {
            res[i] = carry + '0';
            ++i;
        }
        res[i] = '\0';
    }
    reverse_string(res);
    free(num1);
    free(num2);
    return res;

}



int intal_compare(const char* intal1, const char* intal2)
{
/*
First, compare the length of both strings. If there is a difference,
return the one which is larger.
If equal, start iterating from most significant digit. If there is a
difference, return the one possessing lower digit
If you have iterated through whole string, return 0 as they have to be 
equal
*/

    int size1 = strlen(intal1);
    int size2 = strlen(intal2);

    if(size1 > size2)
        return 1;
    if(size1 < size2)
        return -1;
    int i = 0;
    while(intal1[i])
    {
        if(intal1[i] > intal2[i])
            return 1;
        if(intal1[i] < intal2[i])
            return -1;
        ++i;
    }
    return 0;

}


char* intal_diff(const char* intal1, const char* intal2)
{
/*
    The required result is abs(intal1 - intal2). First make sure
     intal1 > intal2 using the previously written intal_compare.
    This is done to avoid negative result. Reverse the strings 
    and start subtracting. Carry forward if difference becomes
    negative. It is vital to remove preceeding zeros at the end.
*/

    //we need copies to work with
    char* num1 = (char*)malloc((strlen(intal1) + 1)* sizeof(char));
    char* num2 = (char*)malloc((strlen(intal2) + 1)* sizeof(char));
    strcpy(num1, intal1);
    strcpy(num2, intal2);


    int comp = intal_compare(num1, num2);
    if(comp == -1)
        swap_strings(&num1, &num2);

    int size1 = strlen(num1);
    int size2 = strlen(num2);

    int max_res_size = max(size1, size2);   
    //maximum difference of two numbers can atmost same as max
    char* res = (char*)malloc((max_res_size + 1)*sizeof(char));
    // +1 for the null character

    reverse_string(num1);
    reverse_string(num2);

    int i = 0;

    int carry = 0;
    while(num2[i])
    {
        int diff = (num1[i] - '0') - (num2[i] - '0') - carry;
        if(diff < 0)
        {
            diff += 10;
            carry = 1;
        }
        else
            carry = 0;

        res[i] = diff + '0';
        ++i;
    } 
    while(num1[i])
    {
        int diff = (num1[i] - '0') - carry;
        if(diff < 0)
        {
            diff += 10;
            carry = 1;
        }
        else
            carry = 0;
        res[i] = diff + '0';
        ++i;
    }

    i -= 1;     //necessary as i will be pointing to null

    while(i > 0 && res[i] == '0')
        --i;
    //removes preceeding zeros. After this i is pointing to 
    //first non zero digit or single 0. 

    res[i+1] = '\0';
    reverse_string(res);
    free(num1);
    free(num2);
    return res;
    
}

char* intal_multiply(const char* intal1, const char* intal2)
{

/*
    O(n**2) implementation: For every digit from right to left in
    intal1, multiply with intal2. Sum all these to find end answer
    To save space, we do cumulative sum, that is for every iteration
    sum with answer. We store in reverse manner to avoid waste of
    space if product is smaller.


*/

    int size1 = strlen(intal1);
    int size2 = strlen(intal2);

    int max_res_size = size1 + size2;   
    //maximum product of two numbers can atmost have m+n digits
    char* res = (char*)malloc((max_res_size + 1)*sizeof(char));
    // +1 for the null character

    for(int i = 0; i < max_res_size; ++i)
        res[i] = '0';
    //initialize with 0;

    int left_shift_outer = 0;
    int left_shift_inner;
    int i = size1-1;
    for(; i >= 0; --i)
    {
        left_shift_inner = 0;
        int j = size2 - 1;
        int carry = 0;
        for(; j >= 0; --j)
        {
            int sum = (intal1[i] - '0')*(intal2[j] - '0') + carry +
             (res[left_shift_outer + left_shift_inner] - '0');
            carry = sum / 10;
            res[left_shift_outer + left_shift_inner] = (sum % 10) + '0';
            left_shift_inner += 1;
        }

        if(carry)
            res[left_shift_outer + left_shift_inner] = carry + '0';
        left_shift_outer += 1;
    }

    i = left_shift_inner + left_shift_outer - 1;
    //at end left_shift_inner + left_shift_outer points to one beyond res
    while(i > 0 && res[i] == '0')
        --i;
    //remove preceeding zeros

    res[i+1] = '\0';

    reverse_string(res);
    return res;
}

static char* intal_div(const char* intal1, const char* intal2)
{

    if(0 == strcmp(intal2, "0"))    //division by zero
        exit(1);

    int comp = intal_compare(intal1, intal2);
    if(comp == -1)      //smaller / bigger
    {
        char* zero = (char*)malloc(2*sizeof(char));
        zero[0] = '0';
        zero[1] = '\0';
        return zero;
    }

    int i = 0;
    char* temp = (char*)malloc(2*sizeof(char));
    temp[0] = intal1[i];
    temp[1] = '\0';

    int size1 = strlen(intal1);

    while(intal_compare(temp, intal2) == -1)
    {
        int n = strlen(temp);
        temp = realloc(temp, n + 2);
        //1 for null character and 1 for new data;
        ++i;
        temp[n] = intal1[i];
        temp[n+1] = '\0';
    }

    char* res = (char*)malloc((size1 + 1)*sizeof(char));
    // +1 for null character
    //quotient can atmost have same digits as dividend
    int res_index = 0;

    while(i < size1)
    {
        int quotient = 0;
        while(intal_compare(temp, intal2) != -1)
        {
            char* prev = temp;
            temp = intal_diff(temp, intal2);
            free(prev);
            ++quotient;
        }
        //if intal_compare directly returns 0, corresponding quotient is 0.

        res[res_index] = quotient + '0';
        ++res_index;


        int n = strlen(temp);

        if((1 == n) && (temp[0] == '0'))
        {
            free(temp);
            temp = (char*)malloc(sizeof(char));
            temp[0] = '\0';
            n = 0;
        }
        //we do not want leading zeros


            ++i;
            temp = realloc(temp, n + 2);
            //1 for null character and 1 for new data;
            temp[n] = intal1[i];
            temp[n+1] = '\0';
        
        

    }
    free(temp);
    res[res_index] = '\0';
    return res;
}

char* intal_mod(const char* intal1, const char* intal2)
{
    char* quotient = intal_div(intal1, intal2);
    char* prod = intal_multiply(quotient , intal2);
    char* remainder = intal_diff(intal1, prod);
    free(quotient);
    free(prod);
    return remainder;
}


char* intal_pow(const char* intal1, unsigned int n)
{
    //we need copies to work with
    char* num1 = (char*)malloc((strlen(intal1) + 1)* sizeof(char));
    strcpy(num1, intal1);

    char* res = (char*)malloc(2*sizeof(char));
    res[0] = '1';
    res[1] = '\0';

    while(n)
    {
        if(n % 2)
        {
            char* prev = res;
            res = intal_multiply(res, num1);
            free(prev);
        }
        char* prev = num1;
        num1 = intal_multiply(num1, num1);
        free(prev);
        n/=2;
    }
    free(num1);
    return res;
}

char* intal_gcd(const char* intal1, const char* intal2)
{
    //we need copies to work with
    char* num1 = (char*)malloc((strlen(intal1) + 1)* sizeof(char));
    char* num2 = (char*)malloc((strlen(intal2) + 1)* sizeof(char));
    strcpy(num1, intal1);
    strcpy(num2, intal2);    

    if(0 == strcmp(num1, "0"))
    {
        free(num1);
        return num2;
    }
    if(0 == strcmp(num2, "0"))
    {
        free(num2);
        return num1;
    }
    while(1)
    {
        if(0 == strcmp(num1, "0"))
            break;
        char* prev = num1;
        num1 = intal_mod(num2, num1);
        free(num2);
        num2 = prev; 
    }

    free(num1);
    return num2;

}

char* intal_fibonacci(unsigned int n)
{
    char* zero = (char*)malloc(2*sizeof(char));
    zero[0] = '0';
    zero[1] = '\0';
    
    if(n == 0)
        return zero;

    char* one = (char*)malloc(2*sizeof(char));
    one[0] = '1';
    one[1] = '\0';

    if(n == 1)
        return one;

    char* a = zero;
    char* b = one;
    char* c;
    for(unsigned int i = 2; i <= n; ++i)
    {
        c = intal_add(a, b);
        free(a);
        a = b;
        b = c;
    }
    free(a);
    return c;

}

static char* convert_to_string(unsigned int val)
{
    if(val == 0)
    {
        char* zero = (char*)malloc(2*sizeof(char));
        zero[0] = '0';
        zero[1] = '\0';
        return zero;   
    }
    int number_of_digits = 0;
    int n = val;
    while(val)
    {
        ++number_of_digits;
        val /= 10;
    }
    char* res = (char*)malloc((number_of_digits + 1)*sizeof(char));
    //+1 for null character
    int i = number_of_digits - 1;
    while(n)
    {
        res[i] = n%10 + '0';
        n/= 10;
        --i;
    }
    res[number_of_digits] = '\0';
    return res;
}




char* intal_factorial(unsigned int n)
{

    char* one = (char*)malloc(2*sizeof(char));
    one[0] = '1';
    one[1] = '\0';

    if(n == 0)
        return one;

    char* res = one;
    char* ans, *string_i;
    for(unsigned int i = 1; i <= n; ++i)
    {
        string_i = convert_to_string(i);
        ans = intal_multiply(string_i, res);
        free(string_i);
        free(res);
        res = ans;
    }
    return res;
}


char* intal_bincoeff(unsigned int n, unsigned int k)
{
    k = ((n-k) < k)?(n-k):k;

	char **dp = (char**) malloc((k+1) * sizeof(char*));
    //assign all to zero
	for(int i = 0; i <= k; i++) 
    {
		dp[i] = (char*) malloc(1001 * sizeof(char));
        dp[i][0] = '0';
        dp[i][1] = '\0';
	}
	dp[0][0] = '1';

    for(int i = 1; i <= n; ++i)
    {
        for(int j = min(i, k); j > 0; --j)
        {
            char* prev = dp[j];
            dp[j] = intal_add(dp[j], dp[j-1]);
            free(prev);
        }
    }

    for(int i = 0; i < k; ++i)
        free(dp[i]);
    char* res = dp[k];
    free(dp);

    return res;

}

int intal_max(char **arr, int n)
{
    int best = 0;
    for(int i = 1; i < n; ++i)
    {
        if(intal_compare(arr[i], arr[best]) == 1)
            best = i;
        //since we want smallest offset, only change if you find something
        //better
    }
    return best;
}

int intal_min(char **arr, int n)
{
    int best = 0;
    for(int i = 1; i < n; ++i)
    {
        if(intal_compare(arr[i], arr[best]) == -1)
            best = i;
        //since we want smallest offset, only change if you find something
        //better
    }
    return best;
}

// Returns the offset of the first occurrence of the key intal in the array.
// Returns -1 if the key is not found.
// 1 <= n <= 1000
int intal_search(char **arr, int n, const char* key)
{
    for(int i = 0; i < n; ++i)
    {
        if(intal_compare(arr[i], key) == 0)
            return i;
    }
    return -1;
}


// Returns the offset of the first occurrence of the key intal in the SORTED array.
// Returns -1 if the key is not found.
// The array is sorted in nondecreasing order.
// 1 <= n <= 1000
// The implementation should be a O(log n) algorithm.
int intal_binsearch(char **arr, int n,const  char* key)
{
    //binary search of the form FFFFTTTT. 
    int low = 0, high = n-1;
    while(low < high)
    {
        int mid = low + (high - low)/2;
        if(intal_compare(arr[mid], key) != -1)
            high = mid;
        else
            low = mid + 1;
    }
    //this will give lower bound, check if lower bound is
    //same as key
    if(intal_compare(arr[low], key) != 0)
        return -1;
    return low;
}


static int partition(char** arr, int start, int end)
{
    //Lomuto partition
    char* pivot = arr[end];
    int i = start;
    for(int j = start; j < end; ++j)
    {
        if(intal_compare(arr[j], pivot) == -1)
        {
            swap_strings(&arr[i], &arr[j]);
            ++i;
        }
    }
    swap_strings(&arr[i], &arr[end]);
    return i;
}

static void intal_sort_util(char** arr, int start, int end)
{
    if(start < end)
    {
        int p = partition(arr, start, end);
        intal_sort_util(arr, start, p - 1);
        intal_sort_util(arr, p+1, end);
    }
}

void intal_sort(char **arr, int n)
{
    intal_sort_util(arr, 0, n-1);
}



char* coin_row_problem(char **arr, int n)
{
    //we dont want to return a pointer of array passed, as
    //user doesnt need his array change
    char* a, *b, *c, *sum;

    a = (char*)malloc(2*sizeof(char));
    a[0] = '0';
    a[1] = '\0';
    if(n == 0)
        return a;

    b = (char*)malloc((strlen(arr[0]) + 1)*sizeof(char));
    strcpy(b, arr[0]);

    if(n == 1)
    {
        free(a);
        return b;
    }

    for(int i = 2; i <= n; ++i)
    {
        sum = intal_add(arr[i-1], a);
        if(intal_compare(b, sum) == 1)
        {
            c = (char*)malloc((strlen(b) + 1)*sizeof(char));
            strcpy(c, b);
        }
        else
        {
            c = (char*)malloc((strlen(sum) + 1)*sizeof(char));
            strcpy(c, sum);
        }

        free(sum);
        free(a);
        a = b;
        b = c;
    }
    free(a);
    return c;


}