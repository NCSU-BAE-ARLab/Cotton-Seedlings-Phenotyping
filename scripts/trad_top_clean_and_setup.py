import os
import shutil

data = 'Example/Data' # SET TO DATA DIRECTORY PATH

# Each tray directory
for tray in os.listdir(data):
    # pd directory
    pd = data + '/' + tray + '/pd'
    # Trad Top point cloud
    file_name = sorted(os.listdir(pd))[-2]

    # Create new Trad Top directory and move Trad Top point cloud to it
    trad_top_dir = pd + '/' + file_name[:-4]
    os.mkdir(trad_top_dir)
    shutil.move(pd + '/' + file_name, trad_top_dir + '/' + file_name)

    # Set input/output files
    input_file = trad_top_dir + '/' + file_name
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
            if not ((values[0] < -0.33 or values[0] > 0.37) or
                    (values[1] < -0.28 or values[1] > 0.25) or
                    (values[2] < 0.37 or values[2] > 0.61) or
                    (values[6] > 250 and values[7] > 250 and values[8] > 250) or
                    (values[6] < 35 and values[7] < 35 and values[8] < 30)):
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