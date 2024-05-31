import pickle

# frequency_3_grams = {}
# with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_3_grams_train.txt", "r") as f:
#     lines = f.readlines()
#     for line in lines:
#         line = line.strip()
#         tokens = line.split(" ")
#         key = tokens[0]
#         value = int(tokens[1])

#         if key in frequency_3_grams:
#             frequency_3_grams[key] += value
#         else:
#             frequency_3_grams[key] = value


# with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_3_grams_train.pkl", "wb") as f:
#     pickle.dump(frequency_3_grams, f)

# write frequency_3_grams to another text file
# with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_3_grams_train.txt", "w") as f:
#     for key, value in frequency_3_grams.items():
#         f.write(f"{key} {value}\n")



frequency_2_grams = {}
with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_2_grams_train.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        line = line.strip()
        tokens = line.split(" ")
        key = tokens[0]
        value = int(tokens[1])

        if key in frequency_2_grams:
            frequency_2_grams[key] += value
        else:
            frequency_2_grams[key] = value


with open("/media/baguette/aislam4/paths/models/Probability-Estimator-For-Visual-Code/src/vocabulary_frequencies/frequency_2_grams_train.pkl", "wb") as f:
    pickle.dump(frequency_2_grams, f)