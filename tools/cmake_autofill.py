import glob
import os.path
import pathlib


# py_generated=directories : finds all directories within a root_dir and its subdirectory
#
# optional: dir=name (subdirectory)
def find_directories(root_dir: str, gen_params: dict, generated_lines: list):
    target_dir = root_dir
    if 'dir' in gen_params:
        target_dir += "/" + gen_params['dir']
    for d in glob.glob(target_dir + "/**", recursive=True):
        d = d.strip("/").strip("\\")
        if not os.path.isfile(d):
            generated_lines.append(d.replace(root_dir, "").replace("\\", "/").strip("/"))


# py_generated=files : finds all files within a root_dir and its sub_dir, with optional extensions
#
# optional: dir=name (subdirectory)
# optional: extensions=h|cpp (multiple extensions split by |)
def find_files(root_dir: str, gen_params: dict, generated_lines: list):
    target_dir = root_dir
    if 'dir' in gen_params:
        target_dir += "/" + gen_params['dir']

    if 'extensions' in gen_params:
        extensions = gen_params['extensions'].split('|')
        for ext in extensions:
            for d in glob.glob(target_dir + f"/**/*.{ext}", recursive=True):
                generated_lines.append(d.replace(root_dir, "").replace("\\", "/").strip("/"))
    else:
        for d in glob.glob(target_dir + "/**/*.*", recursive=True):
            generated_lines.append(d.replace(root_dir, "").replace("\\", "/").strip("/"))


# main parsing method, pass in the path of the CMakeLists.txt file with tags
def update_cmake_file(file_path: str):
    # output lines for file
    out_lines = []

    # open for read
    is_generated = False
    with open(file_path, 'r') as file:
        in_lines = file.readlines()
        generated_block = False

        # go through each line of the file
        for ln in in_lines:
            ln = ln.replace("\n", "")

            # if we're inside a generated block, ignore the input lines, as these will be overwritten
            if generated_block:
                # check for end of generated block
                if ln.find("/py_generated") >= 0:
                    generated_block = False
                    out_lines.append(ln)
                continue

            # add line to output
            out_lines.append(ln)

            # find generated block by keyword
            i = ln.find(" py_generated")
            if i == -1:
                # not found
                continue

            # we're inside a generated block, generate lines
            generated_block = True
            is_generated = True
            generated_lines = []

            # parse generation parameters, key/value pairs
            gen_params = {}
            for key_value in ln[i:].strip().split(' '):
                gen_params[key_value[0:key_value.find('=')]] = key_value[key_value.find('=')+1:]

            # invoke the correct generation method
            generated_type = gen_params["py_generated"]

            if generated_type == "directories":
                find_directories(os.path.dirname(file_path), gen_params, generated_lines)
            elif generated_type == "files":
                find_files(os.path.dirname(file_path), gen_params, generated_lines)

            print(f"{file_path} . ln {len(out_lines)} . {ln[i:]}")

            # get indentation
            indent_space = ""
            for s in ln:
                if s == " ":
                    indent_space += " "
                else:
                    break

            # append generated lines to output
            for gen_ln in generated_lines:
                out_lines.append(indent_space + gen_ln)
                print(f"{file_path} . ln {len(out_lines)} . . .  {gen_ln}")

    # replace file contents
    if is_generated and len(out_lines) > 0:
        with open(file_path, 'w') as file:
            for ln in out_lines:
                file.write(ln + "\n")
            print(f"{file_path} UPDATED")
            print("...")


# generates cmake include folders, files, assets for juce
# uses tags in the form of
#
# py_generated=type param1=value1 ...
# ...
# /py_generated
#
# to detect where to make changes
# see methods at beginning of file for available tag types
if __name__ == '__main__':
    print("UPDATING")
    print("...")

    # check all CMakeLists.txt files of this repo
    count = 0
    for path in pathlib.Path("../").rglob("CMakeLists.txt"):
        update_cmake_file(path.__str__())
        count += 1

    print("...")
    print(f"DONE, checked {count} CMakeLists.txt files")
