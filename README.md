MIM: Maximal Inexact Matches
===

<b>Description</b>: Given two genomes r and q, and a reference and query gene, MIM finds CNEs matches between the
gene positions of the respective chromosomes of r and q.

<b>Installation</b>: To compile MIM, please follow the instructions given in file INSTALL.
```
 MIM <options>
 Standard (Mandatory):
   -r, --ref-genome-file	<str>		FASTA reference genome filename.
   -q, --query-genome-file	<str>		FASTA query genome filename.
   -e, --exons-ref-file		<str>		GTF/GFF exon coordinates for reference genome filename.
   -f, --exons-query-file	<str>		GTF/GFF exon coordinates for query genome filename.
   -g, --ref-gene-file		<str>		GTF/GFF filename containing gene data for reference genome.
   -n, --ref-gene-name		<str>		Name of gene in reference genome in which CNEs will be identified.
   -j, --query-gene-file	<str>		GTF/GFF filename containing gene data for query genome.
   -m, --query-gene-name	<str>		Name of gene in query genome in which CNEs will be identified.
   -l, --min-seq-length		<int>		Minimum length of CNEs.
   -k, --sim-threshold		<dbl>		Threshold of similarity between sequences.
   -o, --output-file		<str>		Output filename with CNEs identified.
 Optional:
   -v, --rev-complement		<int>		Choose 1 to compute reverse complement matches or 0 otherwise. Default: 0.
   -x, --remove-overlaps	<int>		Choose 1 to remove overlapping CNEs or 0 otherwise. Default: 1.
 Number of threads: 
   -T, --threads		<int>		Number of threads to use. Default: 1.







  -r, --ref-file               <str>      FASTA reference filename.
  -q, --query-file             <str>      FASTA query filename.
  -o, --output-file            <str>      Output filename with maximal inexact matches.
  -l, --min-seq-length         <int>      Minimum length of match. Minimum: 50.
  -k, --max-error-size         <int>      Maximum error size between matches.
 Optional:
  -M, --longest-inc-matches    <int>      Choose 1 to return all longest increasing maximal inexact matches
                                          or 0 to return all maximal inexact matches. Default: 0.
  -c, --min-cluster-size       <int>      Minimum number of MIM in each cluster when M=1. Default: 5.
  -v, --rev-complement         <int>      Choose 1 to compute reverse complement matches or 0 otherwise. 
					  Default: 0.
  -z, --ref_start_pos          <int>      Starting position of reference to search. Default: 0.
  -x, --ref_end_pos            <int>      Ending position of reference to search. Default: ref length.
  -n, --query_start_pos        <int>      Starting position of query to search. Default: 0.
  -m, --query_end_pos          <int>      Ending position of query to search. Default: query length.
  -a, --all_mims               <int>      Choose 1 to return all maximal inexact matches or 0 to return
                                          single maximal inexact matches. Default: 0.
 Number of threads: 
  -T, --threads                <int>      Number of threads to use. Default: 1.
```

<b>License</b>: GNU GPLv3 License; Copyright (C) 2017 Lorraine A. K. Ayad, Solon P. Pissis, Dimitris Polychronopoulos

