import sys

# Read the input
student_id = input().strip()

# Extract the student number and check digit
student_number, check_digit = student_id.split('-')

# Convert check digit to integer if it's not a question mark
if check_digit != '?':
    check_digit = int(check_digit)

# Compute the weighted sum of the digits
weighted_sum = sum(int(d) * w for d, w in zip(student_number, [2, 3, 5, 7]) if d != '?')

# Determine the position of the question mark
question_mark_pos = student_number.find('?')

# Case 1: No question mark
if question_mark_pos == -1 and check_digit != '?':
    computed_check_digit = (weighted_sum % 11)
    if computed_check_digit == 10:
        computed_check_digit = 'X'
    if computed_check_digit == check_digit:
        print("VALID")
    else:
        print("INVALID")

# Case 2: Question mark in the check digit position
elif check_digit == '?':
    computed_check_digit = (weighted_sum % 11)
    if computed_check_digit == 10:
        computed_check_digit = 'X'
    print(f"{student_number}-{computed_check_digit}")

# Case 3: Question mark in one of the first four digit positions
else:
    for i in range(10):
        temp_sum = weighted_sum + i * [2, 3, 5, 7][question_mark_pos]
        if temp_sum % 11 == check_digit:
            corrected_student_number = student_number[:question_mark_pos] + str(i) + student_number[question_mark_pos + 1:]
            print(f"{corrected_student_number}-{check_digit}")
            break