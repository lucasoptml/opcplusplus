<?xml version="1.0"?>
<!-- we use the xsl:template as a container element -->
<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

	<!-- suppress text for all modes -->
	<xsl:template match="text()" />
	
	<!-- suppress text for all modes -->
	<xsl:template match="text()" mode="tree" />

	<xsl:template match="Code">
		<html>
			<head>
				<link rel="stylesheet" type="text/css">
					<xsl:attribute name="href">
						<xsl:value-of select="Stylesheet" />
					</xsl:attribute>
				</link>
				<script type="text/javascript">
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

				<div class="tree">
					<div class="title">
						File Contains:
					</div>

					<xsl:apply-templates mode="tree" />
				</div>

				<div class="code">
					<xsl:apply-templates />
				</div>
			</body>
		</html>
		<!-- close the xsl:template element -->
	</xsl:template>


	<xsl:template match="Namespace">
		<div class="namespace">
			<div class="title">
				namespace
			</div>
			<div class="name">
				namespace <xsl:value-of select="Name" />
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

	<xsl:template match="Object">
		<div class="object">
			<div class="title">category</div>
			<div class="name">
				<xsl:value-of select="Category" />
				<xsl:text> </xsl:text>
				<xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates />
		</div>
	</xsl:template>


	<xsl:template match="Namespace" mode="tree">
		<div class="namespace">
			<div class="title">namespace</div>
			<div class="name">
				namespace <xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates mode="tree" />
		</div>
	</xsl:template>

	<xsl:template match="Object" mode="tree">
		<div class="object">
			<div class="title">category</div>
			<div class="name">
				<xsl:value-of select="Category" />
				<xsl:text> </xsl:text>
				<xsl:value-of select="Name" />
			</div>
			<xsl:apply-templates mode="tree" />
		</div>
	</xsl:template>



	<!-- functions -->
	<xsl:template match="Function">
		<div class="function">
			<div class="definition">
				<xsl:for-each select="Modifiers/Modifier">
					<span class="modifier">
						<xsl:if test="Description">
							<xsl:attribute name="onMouseOut">return hideTip();</xsl:attribute>
							<xsl:attribute name="onMouseOver">
								<xsl:text>CreateTip(&apos;</xsl:text>
								<xsl:value-of select="Description" />
								<xsl:text>&apos;,&apos;</xsl:text>
								<xsl:value-of select="Name" />
								<xsl:text>&apos;);</xsl:text>
							</xsl:attribute>
						</xsl:if>
						<xsl:value-of select="Name" />
						<xsl:if test="Value">
							(<span class="value">
								<xsl:text> </xsl:text>
								<xsl:value-of select="Value" />
								<xsl:text> </xsl:text>
							</span>)
						</xsl:if>
					</span>
				</xsl:for-each>
				<span class="statement">
					<xsl:value-of select="Definition" />
					<xsl:text> </xsl:text>
				</span>
			</div>

			<div class="automodifiers">
				<div class="title">automatic modifiers</div>
				<xsl:for-each select="Automatics/Modifier">
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
						<xsl:if test="Value">
							(<span class="value">
								<xsl:text> </xsl:text>
								<xsl:value-of select="Value" />
								<xsl:text> </xsl:text>
							</span>)
						</xsl:if>
					</span>
				</xsl:for-each>
			</div>
		</div>
	</xsl:template>

	<xsl:template match="Data">
		<div class="data">
			<div class="definition">
				<xsl:for-each select="Modifiers/Modifier">
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
						<xsl:if test="Value">
							(<span class="value">
								<xsl:text> </xsl:text>
								<xsl:value-of select="Value" />
								<xsl:text> </xsl:text>
							</span>)
						</xsl:if>
					</span>
				</xsl:for-each>
				<span class="statement">
					<xsl:value-of select="Definition" />
				</span>
			</div>

			<div class="automodifiers">
				<div class="title">automatic modifiers</div>
				<xsl:for-each select="Automatics/Modifier">
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
						<xsl:if test="Value">
							(<span class="value">
								<xsl:text> </xsl:text>
								<xsl:value-of select="Value" />
								<xsl:text> </xsl:text>
							</span>)
						</xsl:if>
					</span>
				</xsl:for-each>
			</div>

		</div>
	</xsl:template>
	

	
</xsl:stylesheet>

