import torch
from torch import nn
import numpy as np
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
import glob, os, sys
import soundfile as sf
import matplotlib.pyplot as plt


class Filter(nn.Module):
    def __init__(self):
        super(Filter, self).__init__()
        kernel = 5
        self.conv = nn.Conv1d(1,1,kernel, bias=False, padding=kernel-1, padding_mode="circular")

    def forward(self, x):
        return self.conv(x)


class SoundData(Dataset):

    def __init__(self, clean_data_path, noisy_data_path):
        self.clean_paths = glob.glob(os.path.join(clean_data_path,"*.wav"))
        self.paths = []
        for path in self.clean_paths:
            self.paths.append([path, path.replace(clean_data_path,noisy_data_path)])

    def __len__(self):
        return len(self.paths)

    def __getitem__(self, idx):
        if torch.is_tensor(idx):
            idx = idx.tolist()

        clean_sound_path, noisy_sound_path = self.paths[idx]

        clean_sound, samplerate = sf.read(clean_sound_path)
        noisy_sound, samplerate = sf.read(noisy_sound_path)

        clean_sound = np.array(clean_sound).reshape((1,1,-1))
        noisy_sound = np.array(noisy_sound).reshape((1,1,-1))

        return torch.Tensor(noisy_sound), torch.Tensor(clean_sound)


dataset = SoundData("clean_testset_wav","noisy_testset_wav")


my_filter = Filter() 
criterion = nn.MSELoss()
optimizer = optim.Adam(my_filter.parameters())

for epoch in range(20):

    running_loss = 0.0
    step = 0

    for i in range(len(dataset)):

        step += 1
        noisy_signals, clean_signals = dataset[i]
        assert noisy_signals.shape == clean_signals.shape
        optimizer.zero_grad()
        outputs = my_filter(noisy_signals)
        
        try:
            loss = criterion(outputs, clean_signals)
        except:
            print(outputs.shape)
            print(clean_signals.shape)

        loss.backward()
        optimizer.step()

        if (i+1)%100==0:
            print("{}/{}".format(i,len(dataset)))
        running_loss += loss.item()

    print("Epoch {} Loss: {}".format(epoch+1, running_loss/step))

print('Finished Training')
print("Weights")
print(my_filter.conv.weight)
