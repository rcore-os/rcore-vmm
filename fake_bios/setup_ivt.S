
# setup ivt function

.globl setup_ivt
setup_ivt:
.code16                                             # Assemble for 16-bit mode
	movw $vector0, %ax
	movw %ax, 0
	movw $0x0000, 2
	movw $vector1, %ax
	movw %ax, 4
	movw $0x0000, 6
	movw $vector2, %ax
	movw %ax, 8
	movw $0x0000, 10
	movw $vector3, %ax
	movw %ax, 12
	movw $0x0000, 14
	movw $vector4, %ax
	movw %ax, 16
	movw $0x0000, 18
	movw $vector5, %ax
	movw %ax, 20
	movw $0x0000, 22
	movw $vector6, %ax
	movw %ax, 24
	movw $0x0000, 26
	movw $vector7, %ax
	movw %ax, 28
	movw $0x0000, 30
	movw $vector8, %ax
	movw %ax, 32
	movw $0x0000, 34
	movw $vector9, %ax
	movw %ax, 36
	movw $0x0000, 38
	movw $vector10, %ax
	movw %ax, 40
	movw $0x0000, 42
	movw $vector11, %ax
	movw %ax, 44
	movw $0x0000, 46
	movw $vector12, %ax
	movw %ax, 48
	movw $0x0000, 50
	movw $vector13, %ax
	movw %ax, 52
	movw $0x0000, 54
	movw $vector14, %ax
	movw %ax, 56
	movw $0x0000, 58
	movw $vector15, %ax
	movw %ax, 60
	movw $0x0000, 62
	movw $vector16, %ax
	movw %ax, 64
	movw $0x0000, 66
	movw $vector17, %ax
	movw %ax, 68
	movw $0x0000, 70
	movw $vector18, %ax
	movw %ax, 72
	movw $0x0000, 74
	movw $vector19, %ax
	movw %ax, 76
	movw $0x0000, 78
	movw $vector20, %ax
	movw %ax, 80
	movw $0x0000, 82
	movw $vector21, %ax
	movw %ax, 84
	movw $0x0000, 86
	movw $vector22, %ax
	movw %ax, 88
	movw $0x0000, 90
	movw $vector23, %ax
	movw %ax, 92
	movw $0x0000, 94
	movw $vector24, %ax
	movw %ax, 96
	movw $0x0000, 98
	movw $vector25, %ax
	movw %ax, 100
	movw $0x0000, 102
	movw $vector26, %ax
	movw %ax, 104
	movw $0x0000, 106
	movw $vector27, %ax
	movw %ax, 108
	movw $0x0000, 110
	movw $vector28, %ax
	movw %ax, 112
	movw $0x0000, 114
	movw $vector29, %ax
	movw %ax, 116
	movw $0x0000, 118
	movw $vector30, %ax
	movw %ax, 120
	movw $0x0000, 122
	movw $vector31, %ax
	movw %ax, 124
	movw $0x0000, 126
	movw $vector32, %ax
	movw %ax, 128
	movw $0x0000, 130
	movw $vector33, %ax
	movw %ax, 132
	movw $0x0000, 134
	movw $vector34, %ax
	movw %ax, 136
	movw $0x0000, 138
	movw $vector35, %ax
	movw %ax, 140
	movw $0x0000, 142
	movw $vector36, %ax
	movw %ax, 144
	movw $0x0000, 146
	movw $vector37, %ax
	movw %ax, 148
	movw $0x0000, 150
	movw $vector38, %ax
	movw %ax, 152
	movw $0x0000, 154
	movw $vector39, %ax
	movw %ax, 156
	movw $0x0000, 158
	movw $vector40, %ax
	movw %ax, 160
	movw $0x0000, 162
	movw $vector41, %ax
	movw %ax, 164
	movw $0x0000, 166
	movw $vector42, %ax
	movw %ax, 168
	movw $0x0000, 170
	movw $vector43, %ax
	movw %ax, 172
	movw $0x0000, 174
	movw $vector44, %ax
	movw %ax, 176
	movw $0x0000, 178
	movw $vector45, %ax
	movw %ax, 180
	movw $0x0000, 182
	movw $vector46, %ax
	movw %ax, 184
	movw $0x0000, 186
	movw $vector47, %ax
	movw %ax, 188
	movw $0x0000, 190
	movw $vector48, %ax
	movw %ax, 192
	movw $0x0000, 194
	movw $vector49, %ax
	movw %ax, 196
	movw $0x0000, 198
	movw $vector50, %ax
	movw %ax, 200
	movw $0x0000, 202
	movw $vector51, %ax
	movw %ax, 204
	movw $0x0000, 206
	movw $vector52, %ax
	movw %ax, 208
	movw $0x0000, 210
	movw $vector53, %ax
	movw %ax, 212
	movw $0x0000, 214
	movw $vector54, %ax
	movw %ax, 216
	movw $0x0000, 218
	movw $vector55, %ax
	movw %ax, 220
	movw $0x0000, 222
	movw $vector56, %ax
	movw %ax, 224
	movw $0x0000, 226
	movw $vector57, %ax
	movw %ax, 228
	movw $0x0000, 230
	movw $vector58, %ax
	movw %ax, 232
	movw $0x0000, 234
	movw $vector59, %ax
	movw %ax, 236
	movw $0x0000, 238
	movw $vector60, %ax
	movw %ax, 240
	movw $0x0000, 242
	movw $vector61, %ax
	movw %ax, 244
	movw $0x0000, 246
	movw $vector62, %ax
	movw %ax, 248
	movw $0x0000, 250
	movw $vector63, %ax
	movw %ax, 252
	movw $0x0000, 254
	movw $vector64, %ax
	movw %ax, 256
	movw $0x0000, 258
	movw $vector65, %ax
	movw %ax, 260
	movw $0x0000, 262
	movw $vector66, %ax
	movw %ax, 264
	movw $0x0000, 266
	movw $vector67, %ax
	movw %ax, 268
	movw $0x0000, 270
	movw $vector68, %ax
	movw %ax, 272
	movw $0x0000, 274
	movw $vector69, %ax
	movw %ax, 276
	movw $0x0000, 278
	movw $vector70, %ax
	movw %ax, 280
	movw $0x0000, 282
	movw $vector71, %ax
	movw %ax, 284
	movw $0x0000, 286
	movw $vector72, %ax
	movw %ax, 288
	movw $0x0000, 290
	movw $vector73, %ax
	movw %ax, 292
	movw $0x0000, 294
	movw $vector74, %ax
	movw %ax, 296
	movw $0x0000, 298
	movw $vector75, %ax
	movw %ax, 300
	movw $0x0000, 302
	movw $vector76, %ax
	movw %ax, 304
	movw $0x0000, 306
	movw $vector77, %ax
	movw %ax, 308
	movw $0x0000, 310
	movw $vector78, %ax
	movw %ax, 312
	movw $0x0000, 314
	movw $vector79, %ax
	movw %ax, 316
	movw $0x0000, 318
	movw $vector80, %ax
	movw %ax, 320
	movw $0x0000, 322
	movw $vector81, %ax
	movw %ax, 324
	movw $0x0000, 326
	movw $vector82, %ax
	movw %ax, 328
	movw $0x0000, 330
	movw $vector83, %ax
	movw %ax, 332
	movw $0x0000, 334
	movw $vector84, %ax
	movw %ax, 336
	movw $0x0000, 338
	movw $vector85, %ax
	movw %ax, 340
	movw $0x0000, 342
	movw $vector86, %ax
	movw %ax, 344
	movw $0x0000, 346
	movw $vector87, %ax
	movw %ax, 348
	movw $0x0000, 350
	movw $vector88, %ax
	movw %ax, 352
	movw $0x0000, 354
	movw $vector89, %ax
	movw %ax, 356
	movw $0x0000, 358
	movw $vector90, %ax
	movw %ax, 360
	movw $0x0000, 362
	movw $vector91, %ax
	movw %ax, 364
	movw $0x0000, 366
	movw $vector92, %ax
	movw %ax, 368
	movw $0x0000, 370
	movw $vector93, %ax
	movw %ax, 372
	movw $0x0000, 374
	movw $vector94, %ax
	movw %ax, 376
	movw $0x0000, 378
	movw $vector95, %ax
	movw %ax, 380
	movw $0x0000, 382
	movw $vector96, %ax
	movw %ax, 384
	movw $0x0000, 386
	movw $vector97, %ax
	movw %ax, 388
	movw $0x0000, 390
	movw $vector98, %ax
	movw %ax, 392
	movw $0x0000, 394
	movw $vector99, %ax
	movw %ax, 396
	movw $0x0000, 398
	movw $vector100, %ax
	movw %ax, 400
	movw $0x0000, 402
	movw $vector101, %ax
	movw %ax, 404
	movw $0x0000, 406
	movw $vector102, %ax
	movw %ax, 408
	movw $0x0000, 410
	movw $vector103, %ax
	movw %ax, 412
	movw $0x0000, 414
	movw $vector104, %ax
	movw %ax, 416
	movw $0x0000, 418
	movw $vector105, %ax
	movw %ax, 420
	movw $0x0000, 422
	movw $vector106, %ax
	movw %ax, 424
	movw $0x0000, 426
	movw $vector107, %ax
	movw %ax, 428
	movw $0x0000, 430
	movw $vector108, %ax
	movw %ax, 432
	movw $0x0000, 434
	movw $vector109, %ax
	movw %ax, 436
	movw $0x0000, 438
	movw $vector110, %ax
	movw %ax, 440
	movw $0x0000, 442
	movw $vector111, %ax
	movw %ax, 444
	movw $0x0000, 446
	movw $vector112, %ax
	movw %ax, 448
	movw $0x0000, 450
	movw $vector113, %ax
	movw %ax, 452
	movw $0x0000, 454
	movw $vector114, %ax
	movw %ax, 456
	movw $0x0000, 458
	movw $vector115, %ax
	movw %ax, 460
	movw $0x0000, 462
	movw $vector116, %ax
	movw %ax, 464
	movw $0x0000, 466
	movw $vector117, %ax
	movw %ax, 468
	movw $0x0000, 470
	movw $vector118, %ax
	movw %ax, 472
	movw $0x0000, 474
	movw $vector119, %ax
	movw %ax, 476
	movw $0x0000, 478
	movw $vector120, %ax
	movw %ax, 480
	movw $0x0000, 482
	movw $vector121, %ax
	movw %ax, 484
	movw $0x0000, 486
	movw $vector122, %ax
	movw %ax, 488
	movw $0x0000, 490
	movw $vector123, %ax
	movw %ax, 492
	movw $0x0000, 494
	movw $vector124, %ax
	movw %ax, 496
	movw $0x0000, 498
	movw $vector125, %ax
	movw %ax, 500
	movw $0x0000, 502
	movw $vector126, %ax
	movw %ax, 504
	movw $0x0000, 506
	movw $vector127, %ax
	movw %ax, 508
	movw $0x0000, 510
	movw $vector128, %ax
	movw %ax, 512
	movw $0x0000, 514
	movw $vector129, %ax
	movw %ax, 516
	movw $0x0000, 518
	movw $vector130, %ax
	movw %ax, 520
	movw $0x0000, 522
	movw $vector131, %ax
	movw %ax, 524
	movw $0x0000, 526
	movw $vector132, %ax
	movw %ax, 528
	movw $0x0000, 530
	movw $vector133, %ax
	movw %ax, 532
	movw $0x0000, 534
	movw $vector134, %ax
	movw %ax, 536
	movw $0x0000, 538
	movw $vector135, %ax
	movw %ax, 540
	movw $0x0000, 542
	movw $vector136, %ax
	movw %ax, 544
	movw $0x0000, 546
	movw $vector137, %ax
	movw %ax, 548
	movw $0x0000, 550
	movw $vector138, %ax
	movw %ax, 552
	movw $0x0000, 554
	movw $vector139, %ax
	movw %ax, 556
	movw $0x0000, 558
	movw $vector140, %ax
	movw %ax, 560
	movw $0x0000, 562
	movw $vector141, %ax
	movw %ax, 564
	movw $0x0000, 566
	movw $vector142, %ax
	movw %ax, 568
	movw $0x0000, 570
	movw $vector143, %ax
	movw %ax, 572
	movw $0x0000, 574
	movw $vector144, %ax
	movw %ax, 576
	movw $0x0000, 578
	movw $vector145, %ax
	movw %ax, 580
	movw $0x0000, 582
	movw $vector146, %ax
	movw %ax, 584
	movw $0x0000, 586
	movw $vector147, %ax
	movw %ax, 588
	movw $0x0000, 590
	movw $vector148, %ax
	movw %ax, 592
	movw $0x0000, 594
	movw $vector149, %ax
	movw %ax, 596
	movw $0x0000, 598
	movw $vector150, %ax
	movw %ax, 600
	movw $0x0000, 602
	movw $vector151, %ax
	movw %ax, 604
	movw $0x0000, 606
	movw $vector152, %ax
	movw %ax, 608
	movw $0x0000, 610
	movw $vector153, %ax
	movw %ax, 612
	movw $0x0000, 614
	movw $vector154, %ax
	movw %ax, 616
	movw $0x0000, 618
	movw $vector155, %ax
	movw %ax, 620
	movw $0x0000, 622
	movw $vector156, %ax
	movw %ax, 624
	movw $0x0000, 626
	movw $vector157, %ax
	movw %ax, 628
	movw $0x0000, 630
	movw $vector158, %ax
	movw %ax, 632
	movw $0x0000, 634
	movw $vector159, %ax
	movw %ax, 636
	movw $0x0000, 638
	movw $vector160, %ax
	movw %ax, 640
	movw $0x0000, 642
	movw $vector161, %ax
	movw %ax, 644
	movw $0x0000, 646
	movw $vector162, %ax
	movw %ax, 648
	movw $0x0000, 650
	movw $vector163, %ax
	movw %ax, 652
	movw $0x0000, 654
	movw $vector164, %ax
	movw %ax, 656
	movw $0x0000, 658
	movw $vector165, %ax
	movw %ax, 660
	movw $0x0000, 662
	movw $vector166, %ax
	movw %ax, 664
	movw $0x0000, 666
	movw $vector167, %ax
	movw %ax, 668
	movw $0x0000, 670
	movw $vector168, %ax
	movw %ax, 672
	movw $0x0000, 674
	movw $vector169, %ax
	movw %ax, 676
	movw $0x0000, 678
	movw $vector170, %ax
	movw %ax, 680
	movw $0x0000, 682
	movw $vector171, %ax
	movw %ax, 684
	movw $0x0000, 686
	movw $vector172, %ax
	movw %ax, 688
	movw $0x0000, 690
	movw $vector173, %ax
	movw %ax, 692
	movw $0x0000, 694
	movw $vector174, %ax
	movw %ax, 696
	movw $0x0000, 698
	movw $vector175, %ax
	movw %ax, 700
	movw $0x0000, 702
	movw $vector176, %ax
	movw %ax, 704
	movw $0x0000, 706
	movw $vector177, %ax
	movw %ax, 708
	movw $0x0000, 710
	movw $vector178, %ax
	movw %ax, 712
	movw $0x0000, 714
	movw $vector179, %ax
	movw %ax, 716
	movw $0x0000, 718
	movw $vector180, %ax
	movw %ax, 720
	movw $0x0000, 722
	movw $vector181, %ax
	movw %ax, 724
	movw $0x0000, 726
	movw $vector182, %ax
	movw %ax, 728
	movw $0x0000, 730
	movw $vector183, %ax
	movw %ax, 732
	movw $0x0000, 734
	movw $vector184, %ax
	movw %ax, 736
	movw $0x0000, 738
	movw $vector185, %ax
	movw %ax, 740
	movw $0x0000, 742
	movw $vector186, %ax
	movw %ax, 744
	movw $0x0000, 746
	movw $vector187, %ax
	movw %ax, 748
	movw $0x0000, 750
	movw $vector188, %ax
	movw %ax, 752
	movw $0x0000, 754
	movw $vector189, %ax
	movw %ax, 756
	movw $0x0000, 758
	movw $vector190, %ax
	movw %ax, 760
	movw $0x0000, 762
	movw $vector191, %ax
	movw %ax, 764
	movw $0x0000, 766
	movw $vector192, %ax
	movw %ax, 768
	movw $0x0000, 770
	movw $vector193, %ax
	movw %ax, 772
	movw $0x0000, 774
	movw $vector194, %ax
	movw %ax, 776
	movw $0x0000, 778
	movw $vector195, %ax
	movw %ax, 780
	movw $0x0000, 782
	movw $vector196, %ax
	movw %ax, 784
	movw $0x0000, 786
	movw $vector197, %ax
	movw %ax, 788
	movw $0x0000, 790
	movw $vector198, %ax
	movw %ax, 792
	movw $0x0000, 794
	movw $vector199, %ax
	movw %ax, 796
	movw $0x0000, 798
	movw $vector200, %ax
	movw %ax, 800
	movw $0x0000, 802
	movw $vector201, %ax
	movw %ax, 804
	movw $0x0000, 806
	movw $vector202, %ax
	movw %ax, 808
	movw $0x0000, 810
	movw $vector203, %ax
	movw %ax, 812
	movw $0x0000, 814
	movw $vector204, %ax
	movw %ax, 816
	movw $0x0000, 818
	movw $vector205, %ax
	movw %ax, 820
	movw $0x0000, 822
	movw $vector206, %ax
	movw %ax, 824
	movw $0x0000, 826
	movw $vector207, %ax
	movw %ax, 828
	movw $0x0000, 830
	movw $vector208, %ax
	movw %ax, 832
	movw $0x0000, 834
	movw $vector209, %ax
	movw %ax, 836
	movw $0x0000, 838
	movw $vector210, %ax
	movw %ax, 840
	movw $0x0000, 842
	movw $vector211, %ax
	movw %ax, 844
	movw $0x0000, 846
	movw $vector212, %ax
	movw %ax, 848
	movw $0x0000, 850
	movw $vector213, %ax
	movw %ax, 852
	movw $0x0000, 854
	movw $vector214, %ax
	movw %ax, 856
	movw $0x0000, 858
	movw $vector215, %ax
	movw %ax, 860
	movw $0x0000, 862
	movw $vector216, %ax
	movw %ax, 864
	movw $0x0000, 866
	movw $vector217, %ax
	movw %ax, 868
	movw $0x0000, 870
	movw $vector218, %ax
	movw %ax, 872
	movw $0x0000, 874
	movw $vector219, %ax
	movw %ax, 876
	movw $0x0000, 878
	movw $vector220, %ax
	movw %ax, 880
	movw $0x0000, 882
	movw $vector221, %ax
	movw %ax, 884
	movw $0x0000, 886
	movw $vector222, %ax
	movw %ax, 888
	movw $0x0000, 890
	movw $vector223, %ax
	movw %ax, 892
	movw $0x0000, 894
	movw $vector224, %ax
	movw %ax, 896
	movw $0x0000, 898
	movw $vector225, %ax
	movw %ax, 900
	movw $0x0000, 902
	movw $vector226, %ax
	movw %ax, 904
	movw $0x0000, 906
	movw $vector227, %ax
	movw %ax, 908
	movw $0x0000, 910
	movw $vector228, %ax
	movw %ax, 912
	movw $0x0000, 914
	movw $vector229, %ax
	movw %ax, 916
	movw $0x0000, 918
	movw $vector230, %ax
	movw %ax, 920
	movw $0x0000, 922
	movw $vector231, %ax
	movw %ax, 924
	movw $0x0000, 926
	movw $vector232, %ax
	movw %ax, 928
	movw $0x0000, 930
	movw $vector233, %ax
	movw %ax, 932
	movw $0x0000, 934
	movw $vector234, %ax
	movw %ax, 936
	movw $0x0000, 938
	movw $vector235, %ax
	movw %ax, 940
	movw $0x0000, 942
	movw $vector236, %ax
	movw %ax, 944
	movw $0x0000, 946
	movw $vector237, %ax
	movw %ax, 948
	movw $0x0000, 950
	movw $vector238, %ax
	movw %ax, 952
	movw $0x0000, 954
	movw $vector239, %ax
	movw %ax, 956
	movw $0x0000, 958
	movw $vector240, %ax
	movw %ax, 960
	movw $0x0000, 962
	movw $vector241, %ax
	movw %ax, 964
	movw $0x0000, 966
	movw $vector242, %ax
	movw %ax, 968
	movw $0x0000, 970
	movw $vector243, %ax
	movw %ax, 972
	movw $0x0000, 974
	movw $vector244, %ax
	movw %ax, 976
	movw $0x0000, 978
	movw $vector245, %ax
	movw %ax, 980
	movw $0x0000, 982
	movw $vector246, %ax
	movw %ax, 984
	movw $0x0000, 986
	movw $vector247, %ax
	movw %ax, 988
	movw $0x0000, 990
	movw $vector248, %ax
	movw %ax, 992
	movw $0x0000, 994
	movw $vector249, %ax
	movw %ax, 996
	movw $0x0000, 998
	movw $vector250, %ax
	movw %ax, 1000
	movw $0x0000, 1002
	movw $vector251, %ax
	movw %ax, 1004
	movw $0x0000, 1006
	movw $vector252, %ax
	movw %ax, 1008
	movw $0x0000, 1010
	movw $vector253, %ax
	movw %ax, 1012
	movw $0x0000, 1014
	movw $vector254, %ax
	movw %ax, 1016
	movw $0x0000, 1018
	movw $vector255, %ax
	movw %ax, 1020
	movw $0x0000, 1022
    ret

.text
.globl vector0
vector0:
	movw $0, %ax
	outw %ax, $0x01
.globl vector1
vector1:
	movw $1, %ax
	outw %ax, $0x01
.globl vector2
vector2:
	movw $2, %ax
	outw %ax, $0x01
.globl vector3
vector3:
	movw $3, %ax
	outw %ax, $0x01
.globl vector4
vector4:
	movw $4, %ax
	outw %ax, $0x01
.globl vector5
vector5:
	movw $5, %ax
	outw %ax, $0x01
.globl vector6
vector6:
	movw $6, %ax
	outw %ax, $0x01
.globl vector7
vector7:
	movw $7, %ax
	outw %ax, $0x01
.globl vector8
vector8:
	movw $8, %ax
	outw %ax, $0x01
.globl vector9
vector9:
	movw $9, %ax
	outw %ax, $0x01
.globl vector10
vector10:
	movw $10, %ax
	outw %ax, $0x01
.globl vector11
vector11:
	movw $11, %ax
	outw %ax, $0x01
.globl vector12
vector12:
	movw $12, %ax
	outw %ax, $0x01
.globl vector13
vector13:
	movw $13, %ax
	outw %ax, $0x01
.globl vector14
vector14:
	movw $14, %ax
	outw %ax, $0x01
.globl vector15
vector15:
	movw $15, %ax
	outw %ax, $0x01
.globl vector16
vector16:
	movw $16, %ax
	outw %ax, $0x01
.globl vector17
vector17:
	movw $17, %ax
	outw %ax, $0x01
.globl vector18
vector18:
	movw $18, %ax
	outw %ax, $0x01
.globl vector19
vector19:
	movw $19, %ax
	outw %ax, $0x01
.globl vector20
vector20:
	movw $20, %ax
	outw %ax, $0x01
.globl vector21
vector21:
	jmp int_0x15
.globl vector22
vector22:
	movw $22, %ax
	outw %ax, $0x01
.globl vector23
vector23:
	movw $23, %ax
	outw %ax, $0x01
.globl vector24
vector24:
	movw $24, %ax
	outw %ax, $0x01
.globl vector25
vector25:
	movw $25, %ax
	outw %ax, $0x01
.globl vector26
vector26:
	movw $26, %ax
	outw %ax, $0x01
.globl vector27
vector27:
	movw $27, %ax
	outw %ax, $0x01
.globl vector28
vector28:
	movw $28, %ax
	outw %ax, $0x01
.globl vector29
vector29:
	movw $29, %ax
	outw %ax, $0x01
.globl vector30
vector30:
	movw $30, %ax
	outw %ax, $0x01
.globl vector31
vector31:
	movw $31, %ax
	outw %ax, $0x01
.globl vector32
vector32:
	movw $32, %ax
	outw %ax, $0x01
.globl vector33
vector33:
	movw $33, %ax
	outw %ax, $0x01
.globl vector34
vector34:
	movw $34, %ax
	outw %ax, $0x01
.globl vector35
vector35:
	movw $35, %ax
	outw %ax, $0x01
.globl vector36
vector36:
	movw $36, %ax
	outw %ax, $0x01
.globl vector37
vector37:
	movw $37, %ax
	outw %ax, $0x01
.globl vector38
vector38:
	movw $38, %ax
	outw %ax, $0x01
.globl vector39
vector39:
	movw $39, %ax
	outw %ax, $0x01
.globl vector40
vector40:
	movw $40, %ax
	outw %ax, $0x01
.globl vector41
vector41:
	movw $41, %ax
	outw %ax, $0x01
.globl vector42
vector42:
	movw $42, %ax
	outw %ax, $0x01
.globl vector43
vector43:
	movw $43, %ax
	outw %ax, $0x01
.globl vector44
vector44:
	movw $44, %ax
	outw %ax, $0x01
.globl vector45
vector45:
	movw $45, %ax
	outw %ax, $0x01
.globl vector46
vector46:
	movw $46, %ax
	outw %ax, $0x01
.globl vector47
vector47:
	movw $47, %ax
	outw %ax, $0x01
.globl vector48
vector48:
	movw $48, %ax
	outw %ax, $0x01
.globl vector49
vector49:
	movw $49, %ax
	outw %ax, $0x01
.globl vector50
vector50:
	movw $50, %ax
	outw %ax, $0x01
.globl vector51
vector51:
	movw $51, %ax
	outw %ax, $0x01
.globl vector52
vector52:
	movw $52, %ax
	outw %ax, $0x01
.globl vector53
vector53:
	movw $53, %ax
	outw %ax, $0x01
.globl vector54
vector54:
	movw $54, %ax
	outw %ax, $0x01
.globl vector55
vector55:
	movw $55, %ax
	outw %ax, $0x01
.globl vector56
vector56:
	movw $56, %ax
	outw %ax, $0x01
.globl vector57
vector57:
	movw $57, %ax
	outw %ax, $0x01
.globl vector58
vector58:
	movw $58, %ax
	outw %ax, $0x01
.globl vector59
vector59:
	movw $59, %ax
	outw %ax, $0x01
.globl vector60
vector60:
	movw $60, %ax
	outw %ax, $0x01
.globl vector61
vector61:
	movw $61, %ax
	outw %ax, $0x01
.globl vector62
vector62:
	movw $62, %ax
	outw %ax, $0x01
.globl vector63
vector63:
	movw $63, %ax
	outw %ax, $0x01
.globl vector64
vector64:
	movw $64, %ax
	outw %ax, $0x01
.globl vector65
vector65:
	movw $65, %ax
	outw %ax, $0x01
.globl vector66
vector66:
	movw $66, %ax
	outw %ax, $0x01
.globl vector67
vector67:
	movw $67, %ax
	outw %ax, $0x01
.globl vector68
vector68:
	movw $68, %ax
	outw %ax, $0x01
.globl vector69
vector69:
	movw $69, %ax
	outw %ax, $0x01
.globl vector70
vector70:
	movw $70, %ax
	outw %ax, $0x01
.globl vector71
vector71:
	movw $71, %ax
	outw %ax, $0x01
.globl vector72
vector72:
	movw $72, %ax
	outw %ax, $0x01
.globl vector73
vector73:
	movw $73, %ax
	outw %ax, $0x01
.globl vector74
vector74:
	movw $74, %ax
	outw %ax, $0x01
.globl vector75
vector75:
	movw $75, %ax
	outw %ax, $0x01
.globl vector76
vector76:
	movw $76, %ax
	outw %ax, $0x01
.globl vector77
vector77:
	movw $77, %ax
	outw %ax, $0x01
.globl vector78
vector78:
	movw $78, %ax
	outw %ax, $0x01
.globl vector79
vector79:
	movw $79, %ax
	outw %ax, $0x01
.globl vector80
vector80:
	movw $80, %ax
	outw %ax, $0x01
.globl vector81
vector81:
	movw $81, %ax
	outw %ax, $0x01
.globl vector82
vector82:
	movw $82, %ax
	outw %ax, $0x01
.globl vector83
vector83:
	movw $83, %ax
	outw %ax, $0x01
.globl vector84
vector84:
	movw $84, %ax
	outw %ax, $0x01
.globl vector85
vector85:
	movw $85, %ax
	outw %ax, $0x01
.globl vector86
vector86:
	movw $86, %ax
	outw %ax, $0x01
.globl vector87
vector87:
	movw $87, %ax
	outw %ax, $0x01
.globl vector88
vector88:
	movw $88, %ax
	outw %ax, $0x01
.globl vector89
vector89:
	movw $89, %ax
	outw %ax, $0x01
.globl vector90
vector90:
	movw $90, %ax
	outw %ax, $0x01
.globl vector91
vector91:
	movw $91, %ax
	outw %ax, $0x01
.globl vector92
vector92:
	movw $92, %ax
	outw %ax, $0x01
.globl vector93
vector93:
	movw $93, %ax
	outw %ax, $0x01
.globl vector94
vector94:
	movw $94, %ax
	outw %ax, $0x01
.globl vector95
vector95:
	movw $95, %ax
	outw %ax, $0x01
.globl vector96
vector96:
	movw $96, %ax
	outw %ax, $0x01
.globl vector97
vector97:
	movw $97, %ax
	outw %ax, $0x01
.globl vector98
vector98:
	movw $98, %ax
	outw %ax, $0x01
.globl vector99
vector99:
	movw $99, %ax
	outw %ax, $0x01
.globl vector100
vector100:
	movw $100, %ax
	outw %ax, $0x01
.globl vector101
vector101:
	movw $101, %ax
	outw %ax, $0x01
.globl vector102
vector102:
	movw $102, %ax
	outw %ax, $0x01
.globl vector103
vector103:
	movw $103, %ax
	outw %ax, $0x01
.globl vector104
vector104:
	movw $104, %ax
	outw %ax, $0x01
.globl vector105
vector105:
	movw $105, %ax
	outw %ax, $0x01
.globl vector106
vector106:
	movw $106, %ax
	outw %ax, $0x01
.globl vector107
vector107:
	movw $107, %ax
	outw %ax, $0x01
.globl vector108
vector108:
	movw $108, %ax
	outw %ax, $0x01
.globl vector109
vector109:
	movw $109, %ax
	outw %ax, $0x01
.globl vector110
vector110:
	movw $110, %ax
	outw %ax, $0x01
.globl vector111
vector111:
	movw $111, %ax
	outw %ax, $0x01
.globl vector112
vector112:
	movw $112, %ax
	outw %ax, $0x01
.globl vector113
vector113:
	movw $113, %ax
	outw %ax, $0x01
.globl vector114
vector114:
	movw $114, %ax
	outw %ax, $0x01
.globl vector115
vector115:
	movw $115, %ax
	outw %ax, $0x01
.globl vector116
vector116:
	movw $116, %ax
	outw %ax, $0x01
.globl vector117
vector117:
	movw $117, %ax
	outw %ax, $0x01
.globl vector118
vector118:
	movw $118, %ax
	outw %ax, $0x01
.globl vector119
vector119:
	movw $119, %ax
	outw %ax, $0x01
.globl vector120
vector120:
	movw $120, %ax
	outw %ax, $0x01
.globl vector121
vector121:
	movw $121, %ax
	outw %ax, $0x01
.globl vector122
vector122:
	movw $122, %ax
	outw %ax, $0x01
.globl vector123
vector123:
	movw $123, %ax
	outw %ax, $0x01
.globl vector124
vector124:
	movw $124, %ax
	outw %ax, $0x01
.globl vector125
vector125:
	movw $125, %ax
	outw %ax, $0x01
.globl vector126
vector126:
	movw $126, %ax
	outw %ax, $0x01
.globl vector127
vector127:
	movw $127, %ax
	outw %ax, $0x01
.globl vector128
vector128:
	movw $128, %ax
	outw %ax, $0x01
.globl vector129
vector129:
	movw $129, %ax
	outw %ax, $0x01
.globl vector130
vector130:
	movw $130, %ax
	outw %ax, $0x01
.globl vector131
vector131:
	movw $131, %ax
	outw %ax, $0x01
.globl vector132
vector132:
	movw $132, %ax
	outw %ax, $0x01
.globl vector133
vector133:
	movw $133, %ax
	outw %ax, $0x01
.globl vector134
vector134:
	movw $134, %ax
	outw %ax, $0x01
.globl vector135
vector135:
	movw $135, %ax
	outw %ax, $0x01
.globl vector136
vector136:
	movw $136, %ax
	outw %ax, $0x01
.globl vector137
vector137:
	movw $137, %ax
	outw %ax, $0x01
.globl vector138
vector138:
	movw $138, %ax
	outw %ax, $0x01
.globl vector139
vector139:
	movw $139, %ax
	outw %ax, $0x01
.globl vector140
vector140:
	movw $140, %ax
	outw %ax, $0x01
.globl vector141
vector141:
	movw $141, %ax
	outw %ax, $0x01
.globl vector142
vector142:
	movw $142, %ax
	outw %ax, $0x01
.globl vector143
vector143:
	movw $143, %ax
	outw %ax, $0x01
.globl vector144
vector144:
	movw $144, %ax
	outw %ax, $0x01
.globl vector145
vector145:
	movw $145, %ax
	outw %ax, $0x01
.globl vector146
vector146:
	movw $146, %ax
	outw %ax, $0x01
.globl vector147
vector147:
	movw $147, %ax
	outw %ax, $0x01
.globl vector148
vector148:
	movw $148, %ax
	outw %ax, $0x01
.globl vector149
vector149:
	movw $149, %ax
	outw %ax, $0x01
.globl vector150
vector150:
	movw $150, %ax
	outw %ax, $0x01
.globl vector151
vector151:
	movw $151, %ax
	outw %ax, $0x01
.globl vector152
vector152:
	movw $152, %ax
	outw %ax, $0x01
.globl vector153
vector153:
	movw $153, %ax
	outw %ax, $0x01
.globl vector154
vector154:
	movw $154, %ax
	outw %ax, $0x01
.globl vector155
vector155:
	movw $155, %ax
	outw %ax, $0x01
.globl vector156
vector156:
	movw $156, %ax
	outw %ax, $0x01
.globl vector157
vector157:
	movw $157, %ax
	outw %ax, $0x01
.globl vector158
vector158:
	movw $158, %ax
	outw %ax, $0x01
.globl vector159
vector159:
	movw $159, %ax
	outw %ax, $0x01
.globl vector160
vector160:
	movw $160, %ax
	outw %ax, $0x01
.globl vector161
vector161:
	movw $161, %ax
	outw %ax, $0x01
.globl vector162
vector162:
	movw $162, %ax
	outw %ax, $0x01
.globl vector163
vector163:
	movw $163, %ax
	outw %ax, $0x01
.globl vector164
vector164:
	movw $164, %ax
	outw %ax, $0x01
.globl vector165
vector165:
	movw $165, %ax
	outw %ax, $0x01
.globl vector166
vector166:
	movw $166, %ax
	outw %ax, $0x01
.globl vector167
vector167:
	movw $167, %ax
	outw %ax, $0x01
.globl vector168
vector168:
	movw $168, %ax
	outw %ax, $0x01
.globl vector169
vector169:
	movw $169, %ax
	outw %ax, $0x01
.globl vector170
vector170:
	movw $170, %ax
	outw %ax, $0x01
.globl vector171
vector171:
	movw $171, %ax
	outw %ax, $0x01
.globl vector172
vector172:
	movw $172, %ax
	outw %ax, $0x01
.globl vector173
vector173:
	movw $173, %ax
	outw %ax, $0x01
.globl vector174
vector174:
	movw $174, %ax
	outw %ax, $0x01
.globl vector175
vector175:
	movw $175, %ax
	outw %ax, $0x01
.globl vector176
vector176:
	movw $176, %ax
	outw %ax, $0x01
.globl vector177
vector177:
	movw $177, %ax
	outw %ax, $0x01
.globl vector178
vector178:
	movw $178, %ax
	outw %ax, $0x01
.globl vector179
vector179:
	movw $179, %ax
	outw %ax, $0x01
.globl vector180
vector180:
	movw $180, %ax
	outw %ax, $0x01
.globl vector181
vector181:
	movw $181, %ax
	outw %ax, $0x01
.globl vector182
vector182:
	movw $182, %ax
	outw %ax, $0x01
.globl vector183
vector183:
	movw $183, %ax
	outw %ax, $0x01
.globl vector184
vector184:
	movw $184, %ax
	outw %ax, $0x01
.globl vector185
vector185:
	movw $185, %ax
	outw %ax, $0x01
.globl vector186
vector186:
	movw $186, %ax
	outw %ax, $0x01
.globl vector187
vector187:
	movw $187, %ax
	outw %ax, $0x01
.globl vector188
vector188:
	movw $188, %ax
	outw %ax, $0x01
.globl vector189
vector189:
	movw $189, %ax
	outw %ax, $0x01
.globl vector190
vector190:
	movw $190, %ax
	outw %ax, $0x01
.globl vector191
vector191:
	movw $191, %ax
	outw %ax, $0x01
.globl vector192
vector192:
	movw $192, %ax
	outw %ax, $0x01
.globl vector193
vector193:
	movw $193, %ax
	outw %ax, $0x01
.globl vector194
vector194:
	movw $194, %ax
	outw %ax, $0x01
.globl vector195
vector195:
	movw $195, %ax
	outw %ax, $0x01
.globl vector196
vector196:
	movw $196, %ax
	outw %ax, $0x01
.globl vector197
vector197:
	movw $197, %ax
	outw %ax, $0x01
.globl vector198
vector198:
	movw $198, %ax
	outw %ax, $0x01
.globl vector199
vector199:
	movw $199, %ax
	outw %ax, $0x01
.globl vector200
vector200:
	movw $200, %ax
	outw %ax, $0x01
.globl vector201
vector201:
	movw $201, %ax
	outw %ax, $0x01
.globl vector202
vector202:
	movw $202, %ax
	outw %ax, $0x01
.globl vector203
vector203:
	movw $203, %ax
	outw %ax, $0x01
.globl vector204
vector204:
	movw $204, %ax
	outw %ax, $0x01
.globl vector205
vector205:
	movw $205, %ax
	outw %ax, $0x01
.globl vector206
vector206:
	movw $206, %ax
	outw %ax, $0x01
.globl vector207
vector207:
	movw $207, %ax
	outw %ax, $0x01
.globl vector208
vector208:
	movw $208, %ax
	outw %ax, $0x01
.globl vector209
vector209:
	movw $209, %ax
	outw %ax, $0x01
.globl vector210
vector210:
	movw $210, %ax
	outw %ax, $0x01
.globl vector211
vector211:
	movw $211, %ax
	outw %ax, $0x01
.globl vector212
vector212:
	movw $212, %ax
	outw %ax, $0x01
.globl vector213
vector213:
	movw $213, %ax
	outw %ax, $0x01
.globl vector214
vector214:
	movw $214, %ax
	outw %ax, $0x01
.globl vector215
vector215:
	movw $215, %ax
	outw %ax, $0x01
.globl vector216
vector216:
	movw $216, %ax
	outw %ax, $0x01
.globl vector217
vector217:
	movw $217, %ax
	outw %ax, $0x01
.globl vector218
vector218:
	movw $218, %ax
	outw %ax, $0x01
.globl vector219
vector219:
	movw $219, %ax
	outw %ax, $0x01
.globl vector220
vector220:
	movw $220, %ax
	outw %ax, $0x01
.globl vector221
vector221:
	movw $221, %ax
	outw %ax, $0x01
.globl vector222
vector222:
	movw $222, %ax
	outw %ax, $0x01
.globl vector223
vector223:
	movw $223, %ax
	outw %ax, $0x01
.globl vector224
vector224:
	movw $224, %ax
	outw %ax, $0x01
.globl vector225
vector225:
	movw $225, %ax
	outw %ax, $0x01
.globl vector226
vector226:
	movw $226, %ax
	outw %ax, $0x01
.globl vector227
vector227:
	movw $227, %ax
	outw %ax, $0x01
.globl vector228
vector228:
	movw $228, %ax
	outw %ax, $0x01
.globl vector229
vector229:
	movw $229, %ax
	outw %ax, $0x01
.globl vector230
vector230:
	movw $230, %ax
	outw %ax, $0x01
.globl vector231
vector231:
	movw $231, %ax
	outw %ax, $0x01
.globl vector232
vector232:
	movw $232, %ax
	outw %ax, $0x01
.globl vector233
vector233:
	movw $233, %ax
	outw %ax, $0x01
.globl vector234
vector234:
	movw $234, %ax
	outw %ax, $0x01
.globl vector235
vector235:
	movw $235, %ax
	outw %ax, $0x01
.globl vector236
vector236:
	movw $236, %ax
	outw %ax, $0x01
.globl vector237
vector237:
	movw $237, %ax
	outw %ax, $0x01
.globl vector238
vector238:
	movw $238, %ax
	outw %ax, $0x01
.globl vector239
vector239:
	movw $239, %ax
	outw %ax, $0x01
.globl vector240
vector240:
	movw $240, %ax
	outw %ax, $0x01
.globl vector241
vector241:
	movw $241, %ax
	outw %ax, $0x01
.globl vector242
vector242:
	movw $242, %ax
	outw %ax, $0x01
.globl vector243
vector243:
	movw $243, %ax
	outw %ax, $0x01
.globl vector244
vector244:
	movw $244, %ax
	outw %ax, $0x01
.globl vector245
vector245:
	movw $245, %ax
	outw %ax, $0x01
.globl vector246
vector246:
	movw $246, %ax
	outw %ax, $0x01
.globl vector247
vector247:
	movw $247, %ax
	outw %ax, $0x01
.globl vector248
vector248:
	movw $248, %ax
	outw %ax, $0x01
.globl vector249
vector249:
	movw $249, %ax
	outw %ax, $0x01
.globl vector250
vector250:
	movw $250, %ax
	outw %ax, $0x01
.globl vector251
vector251:
	movw $251, %ax
	outw %ax, $0x01
.globl vector252
vector252:
	movw $252, %ax
	outw %ax, $0x01
.globl vector253
vector253:
	movw $253, %ax
	outw %ax, $0x01
.globl vector254
vector254:
	movw $254, %ax
	outw %ax, $0x01
.globl vector255
vector255:
	movw $255, %ax
	outw %ax, $0x01

int_0x15:
    mov $memmap, %esi
    add %ebx, %esi
    rep; movsb;
    add $20, %ebx
    sub $20, %edi

    cmp $(memmap_end-memmap), %ebx
    jl tmp0
    mov $0, %ebx
tmp0:
    
    iret

.p2align 2
.data
memmap:
	.long 0x0, 0x0
	.long 0x9fc00, 0x0
	.long 0x1
	
	.long 0x9fc00, 0x0
	.long 0x400, 0x0
	.long 0x2
	
	.long 0xf0000, 0x0
	.long 0x10000, 0x0
	.long 0x2
	
	.long 0x100000, 0x0
	.long 0x7ee0000, 0x0
	.long 0x1
	
	.long 0x7fe0000, 0x0
	.long 0x20000, 0x0
	.long 0x2
	
	.long 0xfffc0000, 0x0
	.long 0x40000, 0x0
	.long 0x2
	
memmap_end:
