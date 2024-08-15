#include "the3.h"

// do not add extra libraries, but you can define helper functions below.

/*
PART 1
you are expected to call recursive_alignment (as the name suggests) recursively to find an alignment.
initial call_count value given to you will be 0.
you should check if call_count >= 1000000, if so, set possible_alignment string to "STACK LIMIT REACHED", return INT_MIN (or anything - it will not be checked).
*/
int overflow_flag = 0;
int recursive_helper(std::string sequence_A, std::string sequence_B, int gap, int mismatch, int match, std::string &possible_alignment, int i, int j, int &call_count)
{
    call_count++;
    if (call_count > 1000000)
    {
        possible_alignment = "STACK LIMIT REACHED";
        overflow_flag = 1;
        return INT_MIN;
    }
    else if (i == 0 && j == 0)
    {
        possible_alignment = "";
        return 0;
    }
    else if (i == 0)
    {
        possible_alignment = std::string(j, '_') + possible_alignment;
        return (-gap) * j;
    }
    else if (j == 0)
    {
        possible_alignment = std::string(i, '.') + possible_alignment;
        return (-gap) * i;
    }
    int flag = 0, case_match, case_A, case_B, score;
    if (i > 0 && j > 0 && sequence_A[i - 1] == sequence_B[j - 1])
    {
        score = match;
        flag = 1;
    }
    else
    {
        score = (-mismatch);
    }
    std::string string_match, string_A, string_B;
    case_match = recursive_helper(sequence_A, sequence_B, gap, mismatch, match, string_match, i - 1, j - 1, call_count) + score;
    case_A = recursive_helper(sequence_A, sequence_B, gap, mismatch, match, string_A, i - 1, j, call_count) - gap;
    case_B = recursive_helper(sequence_A, sequence_B, gap, mismatch, match, string_B, i, j - 1, call_count) - gap;
    if (case_match == INT_MIN || case_A == INT_MIN || case_B == INT_MIN)
    {
        possible_alignment = "STACK LIMIT REACHED";
        return INT_MIN;
    }
    else if (case_match >= case_A && case_match >= case_B)
    {

        if (possible_alignment != "STACK LIMIT REACHED")
        {
            if (flag)
            {
                possible_alignment = string_match + sequence_A[i - 1] + possible_alignment;
            }
            else
            {
                possible_alignment = string_match + '!' + possible_alignment;
            }
        }
        return case_match;
    }
    else if (case_A > case_match && case_A > case_B)
    {
        if (possible_alignment != "STACK LIMIT REACHED")
        {
            possible_alignment = string_A + '.' + possible_alignment;
        }
        return case_A;
    }
    else if (case_B > case_match && case_B > case_A)
    {
        if (possible_alignment != "STACK LIMIT REACHED")
        {
            possible_alignment = string_B + '_' + possible_alignment;
        }
        return case_B;
    }
    if (call_count > 1000000)
    {
        possible_alignment = "STACK LIMIT REACHED";
        overflow_flag = 1;
        return INT_MIN;
    }
}
int recursive_alignment(std::string sequence_A, std::string sequence_B, int gap, int mismatch, int match, std::string &possible_alignment, int call_count)
{
    int highest_alignment_score;
    int size_A = sequence_A.length();
    int size_B = sequence_B.length();
    highest_alignment_score = recursive_helper(sequence_A, sequence_B, gap, mismatch, match, possible_alignment, size_A, size_B, call_count);
    if (overflow_flag)
    {
        possible_alignment = "STACK LIMIT REACHED";
        highest_alignment_score = INT_MIN;
    }
    return highest_alignment_score;
}

/*
PART 2
you are expected to create a dynamic programming table to find the highest alignment score.
then you will need to reconstruct a possible alignment string from the table.
*/
int dp_table_alignment(std::string sequence_A, std::string sequence_B, int gap, int mismatch, int match, std::string &possible_alignment)
{
    int highest_alignment_score;
    int size_A = sequence_A.length();
    int size_B = sequence_B.length();
    std::vector<std::vector<int>> table(size_A + 1, std::vector<int>(size_B + 1, 0));
    for (int k = 0; k <= size_A; k++)
    {
        table[k][0] = -gap * k;
    }
    for (int k = 0; k <= size_B; k++)
    {
        table[0][k] = -gap * k;
    }
    for (int i = 1; i <= size_A; i++)
    {
        for (int j = 1; j <= size_B; j++)
        {
            int case_match, case_A, case_B, score;
            if (sequence_A[i - 1] == sequence_B[j - 1])
            {
                score = match;
            }
            else
            {
                score = (-mismatch);
            }
            case_match = table[i - 1][j - 1] + score;
            case_A = table[i - 1][j] - gap;
            case_B = table[i][j - 1] - gap;
            int temp = std::max(std::max(case_A, case_B), case_match);
            table[i][j] = temp;
        }
    }
    possible_alignment = "";
    int i = size_A, j = size_B;
    while (!(i == 0 && j == 0))
    {
        int case_match, case_A, case_B, case_mismatch;
        if (i == 0)
        {
            while (j > 0)
            {
                possible_alignment = "_" + possible_alignment;
                j--;
            }
            break;
        }
        if (j == 0)
        {
            while (i > 0)
            {
                possible_alignment = "." + possible_alignment;
                i--;
            }
            break;
        }
        case_match = table[i - 1][j - 1] + match;
        case_mismatch = table[i - 1][j - 1] - mismatch;
        case_A = table[i - 1][j] - gap;
        case_B = table[i][j - 1] - gap;
        if (table[i][j] == case_match && sequence_A[i - 1] == sequence_B[j - 1])
        {
            possible_alignment = sequence_A[i - 1] + possible_alignment;
            i--;
            j--;
        }
        else if (table[i][j] == case_mismatch)
        {
            possible_alignment = '!' + possible_alignment;
            i--;
            j--;
        }
        else if (table[i][j] == case_A)
        {
            possible_alignment = '.' + possible_alignment;
            i--;
        }
        else if (table[i][j] == case_B)
        {
            possible_alignment = '_' + possible_alignment;
            j--;
        }
    }
    highest_alignment_score = table[size_A][size_B];
    return highest_alignment_score;
}
