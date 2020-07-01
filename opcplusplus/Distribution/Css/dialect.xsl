<?xml version="1.0"?>
<!-- we use the xsl:template as a container element -->
<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

	<!-- suppress text for all modes -->
	<xsl:template match="text()" />
	
	<xsl:template match="Dialect">
		<html>
			<head>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href">
						<xsl:value-of select="Stylesheet" />
					</xsl:attribute>
				</link>
				<script type="text/javascript">
					<!--<xsl:attribute name="src"><xsl:value-of select="Directory"/><xsl:text>skinnytip.js</xsl:text></xsl:attribute>
					 SkinnyTip (c) Elliott Brueggeman -->
					function CreateTip(text,title)
					{
						Tip(text,TITLE,title);
					}
				</script>
			</head>
			<body>
				<script type="text/javascript">
					<xsl:attribute name="src">
						<xsl:value-of select="Directory"/>
						<xsl:text>wz_tooltip.js</xsl:text>
					</xsl:attribute>
				</script>
				<!-- TODO: add a tree for this...
				<div class="tree">
					<div class="title">
						File Contains:
					</div>
					
					<xsl:apply-templates mode="tree" />
				</div>
				-->
				<div class="dialect">
					<xsl:apply-templates />
				</div>
			</body>
		</html>
		<!-- close the xsl:template element -->
	</xsl:template>
	
	<xsl:template match="Category">
		<div class="category">
			<div class="title">category</div>
			<div class="name">
				Category
				<xsl:text> </xsl:text>
				<xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates />
		</div>
	</xsl:template>

	<xsl:template match="Include">
		<div class="include">
			<div class="title">
				opinclude
			</div>
			<div class="name">
				opinclude <xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates />
		</div>
	</xsl:template>
	
	<xsl:template match="Location">
		<div class="location">
			<div class="title">location</div>
			<div class="name">
				Location
				<xsl:text> </xsl:text>
				<xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates />
		</div>
	</xsl:template>
	
	<xsl:template match="Enumeration">
		<div class="enumeration">
			<div class="title">enumeration</div>
			<div class="name">
				Enumeration
				<xsl:text> </xsl:text>
				<xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates />
		</div>
	</xsl:template>
	
	<!-- functions -->
	<xsl:template match="Note">
		<div class="note">
			<div class="title">note</div>
			
			<div class="name">
					<!--				
					<div class="gotobutton">
					goto code
					<span class="gotopath">
						<xsl:value-of select="Path" />
					</span>
					<span class="gotoline">
						<xsl:value-of select="Line" />
					</span> 
					
				</div>
				-->
				Note <xsl:value-of select="Name" />
			</div>

			<xsl:if test="Modifier">
				<div class="modifiers">
					
					<div class="title">available modifiers</div>
					<xsl:for-each select="Modifier">
						<span class="modifier">
							<xsl:if test="Description">
								<xsl:attribute name="onMouseOver">
									<xsl:text>CreateTip(&apos;</xsl:text>
									<xsl:value-of select="Description" />
									<xsl:text>&apos;,&apos;</xsl:text>
									<xsl:value-of select="Name" />
									<xsl:text>&apos;);</xsl:text>
								</xsl:attribute>
							</xsl:if>
							<xsl:value-of select="Name" />
							
						<xsl:text> </xsl:text>
						</span>
					</xsl:for-each>
				</div>
			</xsl:if>

			<div class="arguments">
				<div class="title">available arguments</div>
				<xsl:for-each select="Argument">
					<span class="argument">
						<xsl:if test="Description">
							<xsl:attribute name="onMouseOver">
								<xsl:text>CreateTip(&apos;</xsl:text>
								<xsl:value-of select="Description" />
								<xsl:text>&apos;,&apos;</xsl:text>
								<xsl:value-of select="Name" />
								<xsl:text>&apos;);</xsl:text>
							</xsl:attribute>
						</xsl:if>
						<xsl:value-of select="Name" />
						
					<xsl:text> </xsl:text>					
					</span>
				</xsl:for-each>
			</div>
		</div>
	</xsl:template>
	
	<xsl:template match="Map">
		<div class="map">
			<div class="title">map</div>
			<div class="name">
				<xsl:value-of select="Type"/> Map <xsl:value-of select="Name" />
			</div>
			<xsl:if test="Modifier">
				<div class="modifiers">
					<div class="title">available modifiers</div>
					<xsl:for-each select="Modifier">
						<span class="modifier">
							<xsl:if test="Description">
								<xsl:attribute name="onMouseOver">
									<xsl:text>CreateTip(&apos;</xsl:text>
									<xsl:value-of select="Description" />
									<xsl:text>&apos;,&apos;</xsl:text>
									<xsl:value-of select="Name" />
									<xsl:text>&apos;);</xsl:text>
								</xsl:attribute>
							</xsl:if>
							  
							<xsl:value-of select="Name" />
							
						<xsl:text> </xsl:text>		
						</span>				
					</xsl:for-each>
				</div>
			</xsl:if>			
			<xsl:apply-templates />
		</div>
	</xsl:template>
	

	
</xsl:stylesheet>

