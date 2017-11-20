library(CNEr); packageDescription ("CNEr", fields = "Version") #"1.12.1". This is for plotCNEDistribution
library(data.table); packageDescription ("data.table", fields = "Version") #"1.10.4". This is for importing files
library(regioneR); packageDescription ("regioneR", fields = "Version") #"1.8.0". For converting coordinates to GRanges; for using randomizeRegions
library(tidyverse); packageDescription ("tidyverse", fields = "Version") #"1.1.1.9000". For using the %>% operator, select columns and create new columns, pull, slice
library(BSgenome.Hsapiens.UCSC.hg38); packageDescription ("BSgenome.Hsapiens.UCSC.hg38", fields = "Version") #"1.4.1". hg38 genome. This is for taking the chromosome length of chr4

# Import output of CNEFinder

realcnes <- toGRanges(fread("~/Downloads/hg38_galGal4_90") %>% 
                                    select(1:4,-V2) %>% 
                                    mutate(chr = "chr4") %>% 
                                    select(chr,V3,V4))

# Get length of chr4 and assign it to the chromosome length of real.cnes (which is chr4)
seqlengths(realcnes) <- seqlengths(BSgenome.Hsapiens.UCSC.hg38) %>% tbl_df() %>% slice(4) %>% pull()

plotCNEDistribution(realcnes) 

ggsave(file="~/Desktop/realcnes.png") # This is a very important function. Here you can play with the defaults and you can export the last_plot using (your) custom settings
# For example: ggsave(file="~/Desktop/realcnes.eps", device = "eps")

# Randomize realcnes 
randomcnes <- randomizeRegions(realcnes,genome = "hg38", per.chromosome = TRUE)
seqlevels(randomcnes) = "chr4"

# Sanity check: They have the same distribution of lengths. Good!
# summary(width(realcnes))
# summary(width(randomcnes))

plotCNEDistribution(cnesrandom) 
ggsave(file="~/Desktop/randomcnes.png") 
