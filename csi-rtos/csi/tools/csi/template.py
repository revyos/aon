cdkprj_template ='''<?xml version="1.0" encoding="UTF-8"?>
<Project Name="hello_world" Version="1">
  <Description></Description>
  <MonitorProgress>
    <FlashOperate>586</FlashOperate>
    <DebugLaunch>42</DebugLaunch>
  </MonitorProgress>
  <Chips>
    <Chip ID="xxx" Version="V1.0.0" IsBasic="false"/>
  </Chips>
  <Boards>
    <Board ID="xxx" Version="V1.0.0" IsBasic="false"/>
  </Boards>
  <Packages>
  </Packages>
  <MergedToYaml>yes</MergedToYaml>
  <Dependencies Name="BuildSet" />
  <DebugSessions>
    <watchExpressions/>
    <memoryExpressions/>
    <statistics/>
    <peripheralTabs/>
    <WatchDisplayFormat/>
    <LocalDisplayFormat/>
    <debugLayout/>
    <memoryTabColSizeExpressions/>
  </DebugSessions>
  <BuildConfigs>
    <BuildConfig Name="BuildSet">
      <Target>
        <ROMBank Selected="1">
          <ROM1>
            <InUse>yes</InUse>
            <Start>0x20000000</Start>
            <Size>0x1000</Size>
          </ROM1>
          <ROM2>
            <InUse>no</InUse>
            <Start />
            <Size />
          </ROM2>
          <ROM3>
            <InUse>no</InUse>
            <Start />
            <Size />
          </ROM3>
          <ROM4>
            <InUse>no</InUse>
            <Start />
            <Size />
          </ROM4>
          <ROM5>
            <InUse>no</InUse>
            <Start />
            <Size />
          </ROM5>
        </ROMBank>
        <RAMBank>
          <RAM1>
            <InUse>no</InUse>
            <Start>0x20001000</Start>
            <Size>0x1000</Size>
            <Init>yes</Init>
          </RAM1>
          <RAM2>
            <InUse>no</InUse>
            <Start />
            <Size />
            <Init>yes</Init>
          </RAM2>
          <RAM3>
            <InUse>no</InUse>
            <Start />
            <Size />
            <Init>yes</Init>
          </RAM3>
          <RAM4>
            <InUse>no</InUse>
            <Start />
            <Size />
            <Init>yes</Init>
          </RAM4>
          <RAM5>
            <InUse>no</InUse>
            <Start />
            <Size />
            <Init>yes</Init>
          </RAM5>
        </RAMBank>
        <CPU>ck803ef</CPU>
        <UseMiniLib>yes</UseMiniLib>
        <Endian>little</Endian>
        <UseHardFloat>no</UseHardFloat>
        <UseEnhancedLRW>no</UseEnhancedLRW>
        <UseContinueBuild>no</UseContinueBuild>
        <UseSemiHost>no</UseSemiHost>
      </Target>
      <Output>
        <OutputName>$(ProjectName)</OutputName>
        <Type>Executable</Type>
        <CreateHexFile>yes</CreateHexFile>
        <CreateBinFile>no</CreateBinFile>
        <Preprocessor>no</Preprocessor>
        <Disassmeble>yes</Disassmeble>
        <CallGraph>no</CallGraph>
        <GenMap>yes</GenMap>
      </Output>
      <User>
        <BeforeCompile>
          <RunUserProg>no</RunUserProg>
          <UserProgName />
        </BeforeCompile>
        <BeforeMake>
          <RunUserProg>yes</RunUserProg>
          <UserProgName>"$(ProjectPath)utilities/pre_build.sh"</UserProgName>
        </BeforeMake>
        <AfterMake>
          <RunUserProg>yes</RunUserProg>
          <UserProgName>"$(ProjectPath)utilities/aft_build.sh"</UserProgName>
        </AfterMake>
      </User>
      <Compiler>
        <Define />
        <Undefine />
        <Optim>Optimize size (-Os)</Optim>
        <DebugLevel>Default (-g)</DebugLevel>
        <IncludePath />
        <OtherFlags />
        <Verbose>no</Verbose>
        <Ansi>no</Ansi>
        <Syntax>no</Syntax>
        <Pedantic>no</Pedantic>
        <PedanticErr>no</PedanticErr>
        <InhibitWarn>no</InhibitWarn>
        <AllWarn>yes</AllWarn>
        <WarnErr>no</WarnErr>
        <OneElfS>no</OneElfS>
        <Fstrict>no</Fstrict>
      </Compiler>
      <Asm>
        <Define />
        <Undefine />
        <IncludePath />
        <OtherFlags />
        <DebugLevel>gdwarf2</DebugLevel>
      </Asm>
      <Linker>
        <Garbage>yes</Garbage>
        <LDFile />
        <LibName />
        <LibPath />
        <OtherFlags />
        <AutoLDFile>no</AutoLDFile>
      </Linker>
      <Debug>
        <LoadApplicationAtStartup>yes</LoadApplicationAtStartup>
        <Connector>ICE</Connector>
        <StopAt>yes</StopAt>
        <StopAtText>main</StopAtText>
        <InitFile>$(ProjectPath)utilities/gdb.init</InitFile>
        <AutoRun>yes</AutoRun>
        <ResetType>Hard Reset</ResetType>
       <SoftResetVal>0</SoftResetVal>
        <ResetAfterLoad>no</ResetAfterLoad>
        <Dumpcore>no</Dumpcore>
        <DumpcoreText>$(ProjectPath)$(ProjectName).cdkcore</DumpcoreText>
        <ConfigICE>
          <IP>localhost</IP>
          <PORT>1025</PORT>
          <CPUNumber>0</CPUNumber>
          <Clock>12000</Clock>
          <Delay>10</Delay>
          <WaitReset>500</WaitReset>
          <DDC>yes</DDC>
          <TRST>no</TRST>
          <DebugPrint>no</DebugPrint>
          <Connect>Normal</Connect>
          <ResetType>Soft Reset</ResetType>
          <SoftResetVal>0</SoftResetVal>
          <RTOSType>Bare Metal</RTOSType>
          <DownloadToFlash>no</DownloadToFlash>
          <ResetAfterConnect>no</ResetAfterConnect>
          <GDBName />
          <GDBServerType>Local</GDBServerType>
          <OtherFlags />
        </ConfigICE>
        <ConfigSIM>
          <SIMTarget>soccfg/cskyv2/rhea802.xml</SIMTarget>
          <OtherFlags />
          <NoGraphic>yes</NoGraphic>
          <Log>no</Log>
          <SimTrace>no</SimTrace>
        </ConfigSIM>
      </Debug>
      <Flash>
        <InitFile>$(ProjectPath)utilities/flash.init</InitFile>
        <Erase>Erase Sectors</Erase>
        <Algorithms Path="$(ProjectPath)utilities/">./utilities/flash.elf</Algorithms>
        <Program>yes</Program>
        <Verify>yes</Verify>
        <ResetAndRun>no</ResetAndRun>
        <ResetType>no</ResetType>
        <SoftResetVal />
        <External>no</External>
        <Command />
        <Arguments />
      </Flash>
    </BuildConfig>
  </BuildConfigs>
</Project>'''

scons_makefile_template = '''CPRE := @
ifeq ($(V),1)
CPRE :=
VERB := --verbose
endif

MK_GENERATED_IMGS_PATH:=generated

.PHONY:startup
startup: all

all:
	$(CPRE) scons $(VERB) -j4
	@echo YoC SDK Done

	@echo [INFO] Create bin files
	$(CPRE) product image $(MK_GENERATED_IMGS_PATH)/images.zip -i $(MK_GENERATED_IMGS_PATH)/data -l -p
	$(CPRE) product image $(MK_GENERATED_IMGS_PATH)/images.zip -e $(MK_GENERATED_IMGS_PATH) -x

.PHONY:flash
flash:
	$(CPRE) product flash $(MK_GENERATED_IMGS_PATH)/images.zip -w prim -f ./<flash.elf> <gdbinit>

.PHONY:flashall
flashall:
	$(CPRE) product flash $(MK_GENERATED_IMGS_PATH)/images.zip -a -f ./<flash.elf> <gdbinit>

sdk:
	$(CPRE) yoc sdk
.PHONY:clean
clean:
	$(CPRE) rm yoc_sdk -rf
	$(CPRE) scons -c
	$(CPRE) find . -name "*.[od]" -delete
'''

scons_sconstruct_template = '''#! /bin/env python

from yoctools import Make

# defconfig = Make(elf='yoc.elf', objcopy='generated/data/prim', objdump='yoc.asm')
defconfig = Make(elf='yoc.elf', objcopy='generated/data/prim')

Export('defconfig')

defconfig.build_components()'''

#test tools config
files_conf_template = '''
header_files:
  - soc: ../../../components/chips/chip_<chip_name>/sys/soc.h               #soc.yaml文件位置
  - pinmux: ../../../components/chips/chip_<chip_name>/include/pinmux.h     #pinmux.h位置
  - pin: ../../../components/csi/include/drv/pin.h                          #pin.h位置
  - driver_path: ../../../components/csi/include/drv                        #driver接口文件位置

yaml_files:
  - board_config: ../../../boards/<chip_name>_evb/board_config.yaml         #board_config.yaml位置
  - soc.yaml: ../../../components/chips/chip_<chip_name>/soc.yaml           #soc.yaml位置
  - test_data: ??????                                                       #test_data.yaml位置
'''

sdk_license_template = '''
                              Apache License
                        Version 2.0, January 2004
                     http://www.apache.org/licenses/

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

1. Definitions.

   "License" shall mean the terms and conditions for use, reproduction,
   and distribution as defined by Sections 1 through 9 of this document.

   "Licensor" shall mean the copyright owner or entity authorized by
   the copyright owner that is granting the License.

   "Legal Entity" shall mean the union of the acting entity and all
   other entities that control, are controlled by, or are under common
   control with that entity. For the purposes of this definition,
   "control" means (i) the power, direct or indirect, to cause the
   direction or management of such entity, whether by contract or
   otherwise, or (ii) ownership of fifty percent (50%) or more of the
   outstanding shares, or (iii) beneficial ownership of such entity.

   "You" (or "Your") shall mean an individual or Legal Entity
   exercising permissions granted by this License.

   "Source" form shall mean the preferred form for making modifications,
   including but not limited to software source code, documentation
   source, and configuration files.

   "Object" form shall mean any form resulting from mechanical
   transformation or translation of a Source form, including but
   not limited to compiled object code, generated documentation,
   and conversions to other media types.

   "Work" shall mean the work of authorship, whether in Source or
   Object form, made available under the License, as indicated by a
   copyright notice that is included in or attached to the work
   (an example is provided in the Appendix below).

   "Derivative Works" shall mean any work, whether in Source or Object
   form, that is based on (or derived from) the Work and for which the
   editorial revisions, annotations, elaborations, or other modifications
   represent, as a whole, an original work of authorship. For the purposes
   of this License, Derivative Works shall not include works that remain
   separable from, or merely link (or bind by name) to the interfaces of,
   the Work and Derivative Works thereof.

   "Contribution" shall mean any work of authorship, including
   the original version of the Work and any modifications or additions
   to that Work or Derivative Works thereof, that is intentionally
   submitted to Licensor for inclusion in the Work by the copyright owner
   or by an individual or Legal Entity authorized to submit on behalf of
   the copyright owner. For the purposes of this definition, "submitted"
   means any form of electronic, verbal, or written communication sent
   to the Licensor or its representatives, including but not limited to
   communication on electronic mailing lists, source code control systems,
   and issue tracking systems that are managed by, or on behalf of, the
   Licensor for the purpose of discussing and improving the Work, but
   excluding communication that is conspicuously marked or otherwise
   designated in writing by the copyright owner as "Not a Contribution."

   "Contributor" shall mean Licensor and any individual or Legal Entity
   on behalf of whom a Contribution has been received by Licensor and
   subsequently incorporated within the Work.

2. Grant of Copyright License. Subject to the terms and conditions of
   this License, each Contributor hereby grants to You a perpetual,
   worldwide, non-exclusive, no-charge, royalty-free, irrevocable
   copyright license to reproduce, prepare Derivative Works of,
   publicly display, publicly perform, sublicense, and distribute the
   Work and such Derivative Works in Source or Object form.

3. Grant of Patent License. Subject to the terms and conditions of
   this License, each Contributor hereby grants to You a perpetual,
   worldwide, non-exclusive, no-charge, royalty-free, irrevocable
   (except as stated in this section) patent license to make, have made,
   use, offer to sell, sell, import, and otherwise transfer the Work,
   where such license applies only to those patent claims licensable
   by such Contributor that are necessarily infringed by their
   Contribution(s) alone or by combination of their Contribution(s)
   with the Work to which such Contribution(s) was submitted. If You
   institute patent litigation against any entity (including a
   cross-claim or counterclaim in a lawsuit) alleging that the Work
   or a Contribution incorporated within the Work constitutes direct
   or contributory patent infringement, then any patent licenses
   granted to You under this License for that Work shall terminate
   as of the date such litigation is filed.

4. Redistribution. You may reproduce and distribute copies of the
   Work or Derivative Works thereof in any medium, with or without
   modifications, and in Source or Object form, provided that You
   meet the following conditions:

   (a) You must give any other recipients of the Work or
       Derivative Works a copy of this License; and

   (b) You must cause any modified files to carry prominent notices
       stating that You changed the files; and

   (c) You must retain, in the Source form of any Derivative Works
       that You distribute, all copyright, patent, trademark, and
       attribution notices from the Source form of the Work,
       excluding those notices that do not pertain to any part of
       the Derivative Works; and

   (d) If the Work includes a "NOTICE" text file as part of its
       distribution, then any Derivative Works that You distribute must
       include a readable copy of the attribution notices contained
       within such NOTICE file, excluding those notices that do not
       pertain to any part of the Derivative Works, in at least one
       of the following places: within a NOTICE text file distributed
       as part of the Derivative Works; within the Source form or
       documentation, if provided along with the Derivative Works; or,
       within a display generated by the Derivative Works, if and
       wherever such third-party notices normally appear. The contents
       of the NOTICE file are for informational purposes only and
       do not modify the License. You may add Your own attribution
       notices within Derivative Works that You distribute, alongside
       or as an addendum to the NOTICE text from the Work, provided
       that such additional attribution notices cannot be construed
       as modifying the License.

   You may add Your own copyright statement to Your modifications and
   may provide additional or different license terms and conditions
   for use, reproduction, or distribution of Your modifications, or
   for any such Derivative Works as a whole, provided Your use,
   reproduction, and distribution of the Work otherwise complies with
   the conditions stated in this License.

5. Submission of Contributions. Unless You explicitly state otherwise,
   any Contribution intentionally submitted for inclusion in the Work
   by You to the Licensor shall be under the terms and conditions of
   this License, without any additional terms or conditions.
   Notwithstanding the above, nothing herein shall supersede or modify
   the terms of any separate license agreement you may have executed
   with Licensor regarding such Contributions.

6. Trademarks. This License does not grant permission to use the trade
   names, trademarks, service marks, or product names of the Licensor,
   except as required for reasonable and customary use in describing the
   origin of the Work and reproducing the content of the NOTICE file.

7. Disclaimer of Warranty. Unless required by applicable law or
   agreed to in writing, Licensor provides the Work (and each
   Contributor provides its Contributions) on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
   implied, including, without limitation, any warranties or conditions
   of TITLE, NON-INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A
   PARTICULAR PURPOSE. You are solely responsible for determining the
   appropriateness of using or redistributing the Work and assume any
   risks associated with Your exercise of permissions under this License.

8. Limitation of Liability. In no event and under no legal theory,
   whether in tort (including negligence), contract, or otherwise,
   unless required by applicable law (such as deliberate and grossly
   negligent acts) or agreed to in writing, shall any Contributor be
   liable to You for damages, including any direct, indirect, special,
   incidental, or consequential damages of any character arising as a
   result of this License or out of the use or inability to use the
   Work (including but not limited to damages for loss of goodwill,
   work stoppage, computer failure or malfunction, or any and all
   other commercial damages or losses), even if such Contributor
   has been advised of the possibility of such damages.

9. Accepting Warranty or Additional Liability. While redistributing
   the Work or Derivative Works thereof, You may choose to offer,
   and charge a fee for, acceptance of support, warranty, indemnity,
   or other liability obligations and/or rights consistent with this
   License. However, in accepting such obligations, You may act only
   on Your own behalf and on Your sole responsibility, not on behalf
   of any other Contributor, and only if You agree to indemnify,
   defend, and hold each Contributor harmless for any liability
   incurred by, or claims asserted against, such Contributor by reason
   of your accepting any such warranty or additional liability.

END OF TERMS AND CONDITIONS

APPENDIX: How to apply the Apache License to your work.

   To apply the Apache License to your work, attach the following
   boilerplate notice, with the fields enclosed by brackets "{}"
   replaced with your own identifying information. (Don't include
   the brackets!)  The text should be enclosed in the appropriate
   comment syntax for the file format. We also recommend that a
   file or class name and description of purpose be included on the
   same "printed page" as the copyright notice for easier
   identification within third-party archives.

Copyright {yyyy} {name of copyright owner}

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.'''

qeum_template = '''<Board name="smartl" arch="csky" cskysimv="2.0">
    <cpu_info cpu="ck805ef" endian="little" abi="abiv2"></cpu_info>
    <dyndev shm="on">
    </dyndev>
    <memory>
        <mem name="smart_inst_mem" addr="0x0" size ="0x01000000" attr ="MEM_RAM"></mem>
        <mem name="smart_data_mem" addr="0x20000000" size ="0x01000000" attr ="MEM_RAM"></mem>
        <mem name="smart_outchip_mem" addr="0x50000000" size ="0x00800000" attr ="MEM_RAM"></mem>
        <mem name="smart_sys_mem" addr="0x60000000" size ="0x01000000" attr ="MEM_RAM"></mem>
    </memory>
    <cpu_prop>
        <cpu name="pctrace"                 value="off"> on/off </cpu>
        <cpu name="elrw"                    value="on"> on/off </cpu>
        <cpu name="mem_prot"                value="mpu"> no/mmu/mpu </cpu>
        <cpu name="unaligned_access"        value="off"> on/off </cpu>
    </cpu_prop>
    <Description> This file is used for dynamic configuration
        <AutoDescription>
            CPU Type: ck805ef Little Endian
            Memory Sections:
                smart_inst_mem, Start: 0x0, Length: 0x1000000
                smart_data_mem, Start: 0x20000000, Length: 0x40000
                smart_outchip_mem, Start: 0x50000000, Length: 0x800000
                smart_sys_mem, Start: 0x60000000, Length: 0x1000000
        </AutoDescription>
    </Description>
</Board>'''


svc_xml_template = '''
<com.csky.cds.peripheral>
  <config Version="0.0.1"/>
  <ChipConfig>
    <CodeGenerateConfig>
      <OutputFileName>$(ProjectPath)/chip_config.c</OutputFileName>
      <IncludeFile>stdio.h</IncludeFile>
      <IncludeFile>stdint.h</IncludeFile>
      <IncludeFile>drv/gpio.h</IncludeFile>
      <IncludeFile>sys_clk.h</IncludeFile>
      <IncludeFile>drv/pin.h</IncludeFile>
      <DLLConfig>
        <DLLFile>$(ProjectPath)/utilities/chip_config.dll</DLLFile>
        <InitFunc>OnInit</InitFunc>
        <UninitFunc>OnUninit</UninitFunc>
        <GetErrorFunc>GetErrorMsg</GetErrorFunc>
      </DLLConfig>
    </CodeGenerateConfig>
    <ChipView>
      <ChipName>CH2201</ChipName>
      <ChipShape>CVS_LOOK_SQUARE</ChipShape>
      <PinCount>1</PinCount>
      <GenCodeFuncName>pin_init_code_gen</GenCodeFuncName>
      <Pins>
      </Pins>
    </ChipView>
    <ChipPropertiesView>
      <ChipPins>
        <label>I/O ports</label>
      </ChipPins>
    </ChipPropertiesView>
  </ChipConfig>
</com.csky.cds.peripheral>
'''

svc_chip_config_template ='''
/**
  * Auto Generated by CDK.
  * Do not modify this file, and any manual changes will be erased!!!
  */


#include "stdio.h"
#include "stdint.h"
#include "drv/gpio.h"
#include "sys_clk.h"
#include "drv/pin.h"


void PIN_init(void)
{
;
}

void __attribute__ ((weak)) __ChipInitHandler(void)
{
    PIN_init();
}
'''
