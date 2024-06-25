import os

# Set input/output files
input_file = 'example.ply' # SET TO FILE PATH
output_file = input_file[:-4] + '_filtered.ply'
temp_output_file = output_file + '.tmp'

# Clean data
with open(input_file, 'r') as f_in, open(temp_output_file, 'w') as f_out:
    # Write header lines
    for line in f_in:
        f_out.write(line)
        if line.strip() == 'end_header':
            break

    # Write filtered data
    count = 0
    for line in f_in:
        values = list(map(float, line.split()))
        if not ((values[0] < -0.21 or values[0] > 0.25) or
                (values[1] < -0.2 or values[1] > 0.2) or
                (values[2] < 0.5 or values[2] > 0.67) or
                (values[6] > 250 and values[7] > 250)):
            f_out.write(line)
            count += 1

# Write element vertex header
with open(temp_output_file, 'r') as f_in, open(output_file, 'w') as f_out:
    for line in f_in:
        if line.strip().startswith('element vertex'):
            f_out.write(f'element vertex {count}\n')
        else:
            f_out.write(line)

# Remove temp file
os.remove(temp_output_file)
