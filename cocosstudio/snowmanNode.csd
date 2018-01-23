<GameFile>
  <PropertyGroup Name="snowmanNode" Type="Node" ID="41cd91a9-ec2e-431c-b451-0cdba9f9f673" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="24" Speed="1.0000">
        <Timeline ActionTag="-404604209" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="24" Value="255">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="-224029088" Property="FileData">
          <TextureFrame FrameIndex="0" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_00.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="3" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_01.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="6" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_02.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="9" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_03.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="12" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_04.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="15" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_05.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="18" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_06.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="21" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_07.png" Plist="rungame.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="24" Tween="False">
            <TextureFile Type="PlistSubImage" Path="flame1_00.png" Plist="rungame.plist" />
          </TextureFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="SnowManRoot" Tag="49" ctype="GameNodeObjectData">
        <Size X="0.0000" Y="0.0000" />
        <Children>
          <AbstractNodeData Name="snowballPos" ActionTag="-1412529517" Tag="104" IconVisible="True" LeftMargin="-2.6300" RightMargin="2.6300" TopMargin="24.9400" BottomMargin="-24.9400" ctype="SingleNodeObjectData">
            <Size X="0.0000" Y="0.0000" />
            <AnchorPoint />
            <Position X="-2.6300" Y="-24.9400" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="snowball1" ActionTag="729975585" Tag="101" IconVisible="False" LeftMargin="-49.6265" RightMargin="-44.3735" TopMargin="-16.0621" BottomMargin="-65.9379" ctype="SpriteObjectData">
            <Size X="94.0000" Y="82.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-2.6265" Y="-24.9379" />
            <Scale ScaleX="0.8045" ScaleY="0.8045" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="snow_ball.png" Plist="rungame.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="snowball2" ActionTag="169786005" Tag="102" IconVisible="False" LeftMargin="-49.6300" RightMargin="-44.3700" TopMargin="-16.0600" BottomMargin="-65.9400" ctype="SpriteObjectData">
            <Size X="94.0000" Y="82.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-2.6300" Y="-24.9400" />
            <Scale ScaleX="0.8045" ScaleY="0.8045" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="snow_ball.png" Plist="rungame.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="snowball3" ActionTag="-1189719520" Tag="103" IconVisible="False" LeftMargin="-49.6300" RightMargin="-44.3700" TopMargin="-16.0600" BottomMargin="-65.9400" ctype="SpriteObjectData">
            <Size X="94.0000" Y="82.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-2.6300" Y="-24.9400" />
            <Scale ScaleX="0.8045" ScaleY="0.8045" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="snow_ball.png" Plist="rungame.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="SnowMan" ActionTag="-404604209" Tag="73" IconVisible="False" LeftMargin="-23.0001" RightMargin="-22.9999" TopMargin="-23.0000" BottomMargin="-23.0000" ctype="SpriteObjectData">
            <Size X="195.0000" Y="212.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="-0.0001" Y="0.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="snow_man.png" Plist="rungame.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="flame" ActionTag="-224029088" Tag="83" IconVisible="False" LeftMargin="-23.0000" RightMargin="-23.0000" TopMargin="-23.0000" BottomMargin="-23.0000" ctype="SpriteObjectData">
            <Size X="240.0000" Y="220.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position />
            <Scale ScaleX="1.2078" ScaleY="1.2078" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="0.0000" Y="0.0000" />
            <FileData Type="PlistSubImage" Path="flame1_00.png" Plist="rungame.plist" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>