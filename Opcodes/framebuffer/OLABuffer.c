/*
 
 OLABuffer.c
 Framebuffer
 
 Created by Edward Costello on 10/06/2015.
 Copyright (c) 2015 Edward Costello.
 
 This file is part of Csound.
 
 The Csound Library is free software; you can redistribute it
 and/or modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 Csound is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with Csound; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 02111-1307 USA
 */

#include "OLABuffer.h"
#include <tgmath.h>

void OLABuffer_checkArgumentSanity(CSOUND *csound, OLABuffer *self);

int OLABuffer_initialise(CSOUND *csound, OLABuffer *self)
{
    OLABuffer_checkArgumentSanity(csound, self);
    self->inputArray = (ARRAYDAT *)self->inputArgument;
    self->frameSamplesCount = self->inputArray->sizes[0];
    self->framesCount = *self->overlapArgument;
    self->overlapSamplesCount = self->frameSamplesCount / self->framesCount;
    csound->AuxAlloc(csound, self->frameSamplesCount * self->framesCount * sizeof(MYFLT), &self->frameSamplesMemory);
    csound->AuxAlloc(csound, self->framesCount * sizeof(MYFLT *), &self->framePointerMemory);
    self->frames = self->framePointerMemory.auxp;
    self->ksmps = csound->GetKsmps(csound);
   
    u_int32_t i;
    for (i = 0; i < self->framesCount; ++i) {
        
        self->frames[i] = &((MYFLT *)self->frameSamplesMemory.auxp)[i * self->frameSamplesCount];
    }
    
    self->overlapSampleIndex = self->overlapSamplesCount;
    
    return OK;
}

void OLABuffer_writeFrame(OLABuffer *self, MYFLT *inputFrame, u_int32_t frameIndex)
{
    u_int32_t firstHalfOffset = self->overlapSamplesCount * frameIndex;
    u_int32_t firstHalfCount = self->frameSamplesCount - firstHalfOffset;
    u_int32_t secondHalfCount = self->frameSamplesCount - firstHalfCount;
    memcpy(&self->frames[frameIndex][firstHalfOffset], inputFrame, firstHalfCount * sizeof(MYFLT));
    memcpy(self->frames[frameIndex], &inputFrame[firstHalfCount], secondHalfCount * sizeof(MYFLT));
}

void OLABuffer_readFrame(OLABuffer *self, MYFLT *outputFrame, u_int32_t outputFrameOffset,
                         u_int32_t olaBufferOffset, u_int32_t samplesCount)
{
    memcpy(&outputFrame[outputFrameOffset], &self->frames[0][olaBufferOffset], samplesCount * sizeof(MYFLT));
   
    u_int32_t i, j;
    for (i = 1; i < self->framesCount; ++i) {
        
        for (j = 0; j < samplesCount; ++j) {
            
            outputFrame[j + outputFrameOffset] += self->frames[i][j + olaBufferOffset];
        }
    }
}

int OLABuffer_process(CSOUND *csound, OLABuffer *self)
{
    u_int32_t nextKPassSampleIndex = (self->readSampleIndex + self->ksmps) % self->overlapSamplesCount;
    
    if (nextKPassSampleIndex == 0) {
        
        OLABuffer_writeFrame(self, self->inputArray->data, self->frameIndex);
        OLABuffer_readFrame(self, self->outputArgument, 0, self->readSampleIndex, self->ksmps);
        self->frameIndex++;
        self->frameIndex %= self->framesCount;
    }
    else if (nextKPassSampleIndex < self->overlapSampleIndex) {
        
        u_int32_t firstHalfCount = self->overlapSamplesCount - self->overlapSampleIndex;
        
        if (firstHalfCount != 0) {
            
            OLABuffer_readFrame(self, self->outputArgument, 0, self->readSampleIndex, firstHalfCount);
        }
    
        OLABuffer_writeFrame(self, self->inputArray->data, self->frameIndex);
        
        u_int32_t secondHalfCount = self->ksmps - firstHalfCount;
        
        if (secondHalfCount != 0) {
            
            OLABuffer_readFrame(self, self->outputArgument, firstHalfCount, self->readSampleIndex, secondHalfCount);
        }
        
        self->frameIndex++;
        self->frameIndex %= self->framesCount;
    }
    else {
        
        OLABuffer_readFrame(self, self->outputArgument, 0, self->readSampleIndex, self->ksmps);
    }

    self->overlapSampleIndex += self->ksmps;
    self->overlapSampleIndex %= self->overlapSamplesCount;
    self->readSampleIndex += self->ksmps;
    self->readSampleIndex %= self->frameSamplesCount;
   
    return OK;
}

void OLABuffer_checkArgumentSanity(CSOUND *csound, OLABuffer *self)
{
    MYFLT overlapCount = *self->overlapArgument;
    
    if (floor(overlapCount) != overlapCount) {
        
<<<<<<< HEAD
      csound->Die(csound, Str("olabuffer: Error, overlap factor must be an integer"));
=======
        csound->Die(csound, Str("olabuffer: Error, overlap factor must be an integer"));
>>>>>>> 32deba15210df32001b84f634aa0bcf2399c89aa
    }
    
    ARRAYDAT *array = (ARRAYDAT *) self->inputArgument;
    
    if (array->dimensions != 1) {
        
<<<<<<< HEAD
      csound->Die(csound, Str("olabuffer: Error, k-rate array must be one dimensional"));
=======
        csound->Die(csound, Str("olabuffer: Error, k-rate array must be one dimensional"));
>>>>>>> 32deba15210df32001b84f634aa0bcf2399c89aa
    }
    
    u_int32_t frameSampleCount = array->sizes[0];
    
    if (frameSampleCount <= (int)overlapCount) {
        
<<<<<<< HEAD
      csound->Die(csound, Str("olabuffer: Error, k-rate array size must be larger than ovelap factor"));
=======
        csound->Die(csound, Str("olabuffer: Error, k-rate array size must be larger than ovelap factor"));
>>>>>>> 32deba15210df32001b84f634aa0bcf2399c89aa
    }
    
    if (frameSampleCount % (int)overlapCount != 0) {
        
<<<<<<< HEAD
      csound->Die(csound, Str("olabuffer: Error, overlap factor must be an integer multiple of k-rate array size"));
=======
        csound->Die(csound, Str("olabuffer: Error, overlap factor must be an integer multiple of k-rate array size"));
>>>>>>> 32deba15210df32001b84f634aa0bcf2399c89aa
    }
    
    if (frameSampleCount / (int)overlapCount < csound->GetKsmps(csound)) {
        
<<<<<<< HEAD
      csound->Die(csound, Str("olabuffer: Error, k-rate array size divided by overlap factor must be larger than or equal to ksmps"));
=======
        csound->Die(csound, Str("olabuffer: Error, k-rate array size divided by overlap factor must be larger than or equal to ksmps"));
>>>>>>> 32deba15210df32001b84f634aa0bcf2399c89aa
    }
}