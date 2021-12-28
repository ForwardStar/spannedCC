import os
import time
import threading
import tarfile

headers = {'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'}

def download(url, path):
    try:
        from tqdm import tqdm
    except:
        from pip._internal import main
        os.system(f'pip3 install tqdm')
    from urllib.request import urlopen, Request
    blocksize = 1024 * 8
    blocknum = 0
    retry_times = 0
    while True:
        try:
            with urlopen(Request(url, headers=headers), timeout=3) as resp:
                total = resp.info().get("content-length", None)
                with tqdm(
                    unit="B",
                    unit_scale=True,
                    miniters=1,
                    unit_divisor=1024,
                    total=total if total is None else int(total),
                ) as t, path.open("wb") as f:
                    block = resp.read(blocksize)
                    while block:
                        f.write(block)
                        blocknum += 1
                        t.update(len(block))
                        block = resp.read(blocksize)
            break
        except KeyboardInterrupt:
            if path.is_file():
                path.unlink()
            raise
        except:
            retry_times += 1
            if retry_times >= 20:
                break
            print("Timed out, retrying...")
    if retry_times >= 20:
        if path.is_file():
            path.unlink()
        raise ConnectionError("bad internet connection, check it and retry.")

def showProcess():
    print(waiting_message, end="  ")
    while is_finished is False:
        print('\b-', end='')
        time.sleep(0.05)
        print('\b\\', end='')
        time.sleep(0.05)
        print('\b|', end='')
        time.sleep(0.05)
        print('\b/', end='')
        time.sleep(0.05)
    if is_finished is True:
        print('\bdone')
    else:
        print('\berror!')
    
def takeThird(triple):
    return triple[2]

def move_data_file(source, destination):
    source = open(os.path.join(os.path.join('datasets', source), "out." + source), "r")
    lines = source.readlines()
    destination = open(destination, "w")
    destination.writelines(lines)

def normalize(filename):
    lines = open(filename, "r").readlines()
    contents = list()

    for line in lines:
        # omit the comments
        if '%' in line:
            continue
        
        # omit the multiplicity of edges
        line = line.split()
        contents.append([line[0], line[1], int(line[len(line) - 1])])
    
    # normalize
    contents.sort(key=takeThird)
    contents[0].append(0)
    for i in range(1, len(contents)):
        if contents[i][2] == contents[i - 1][2]:
            contents[i].append(contents[i - 1][3])
        else:
            contents[i].append(contents[i - 1][3] + 1)

    # wrap up
    text = ""
    for line in contents:
        text += line[0] + " " + line[1] + " " + str(line[3]) + "\n"
    open(filename, "w").write(text)

if __name__ == "__main__":
    '''
    # Download datasets
    if os.path.isdir("datasets") is False or len(os.listdir("datasets")) == 0:
        if os.path.isdir("datasets") is False:
            os.mkdir("datasets")
        print("Downloading datasets...")
        download("http://konect.cc/files/download.tsv.dblp_coauthor.tar.bz2", "datasets/download.tsv.dblp_coauthor.tar.bz2")
    '''

    # Extract all datasets
    file_ls = os.listdir("datasets")
    for file in file_ls:
        if file.endswith(".tar.bz2"):
            archive = tarfile.open(os.path.join("datasets", file), "r:bz2")
            archive.extractall("datasets")
            os.remove(os.path.join("datasets", file))

    # select a target graph dataset
    file_ls = os.listdir('datasets')
    count = 0
    print("Datasets:")
    for file in file_ls:
        print(str(count) + ".", file)
        count += 1
    user_input = input("Select a graph dataset (0-" + str(len(file_ls) - 1) + "): ")

    # move data file
    if user_input.strip() in [str(i) for i in range(len(file_ls))]:
        waiting_message = 'Copying dataset to "graph.txt"...'
        is_finished = False
        thread_move_data_file = threading.Thread(target=showProcess)
        thread_move_data_file.start()
        move_data_file(file_ls[int(user_input)], "graph.txt")
        is_finished = True
        thread_move_data_file.join()
    else:
        print("Invalid input! Program terminated.")
        exit()
    
    # normalize the graph
    waiting_message = "Normalizing the graph..."
    is_finished = False
    thread_normalize = threading.Thread(target=showProcess)
    thread_normalize.start()
    normalize("graph.txt")
    is_finished = True
    thread_normalize.join()