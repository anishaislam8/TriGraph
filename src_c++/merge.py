frequency_3_grams = {}
with open("vocabulary_frequencies/frequency_3_grams_train_all.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        tokens = line.split(" ")
        key = tokens[0]
        value = int(tokens[1])

        if key in frequency_3_grams:
            frequency_3_grams[key] += value
        else:
            frequency_3_grams[key] = value

# write frequency_3_grams to another text file
with open("vocabulary_frequencies/frequency_3_grams_train.txt", "w") as f:
    for key, value in frequency_3_grams.items():
        f.write(f"{key} {value}\n")