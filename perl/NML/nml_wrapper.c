/* This is simple demonstration of how to use expat. This program
   reads an XML document from standard input and writes a line with
   the name of each element to standard output indenting child
   elements by one tab stop more than their parent element.
   It must be used with Expat compiled for UTF-8 output.
*/

#include <stdio.h>
#include <string.h>
#include "expat.h"

#if defined(__amigaos__) && defined(__USE_INLINE__)
#include <proto/expat.h>
#endif

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

typedef struct
{
    char name[128];
    char tag[128];
    char total[64];
    char free[64];
    int matched;
} VOLUME_INFO;

static void XMLCALL startVolumeElement(void *userData, const char *name, const char **atts)
{
    int att = 0;
    char *volume = (char *)userData;
    if (strcmp(name, "Volume") == 0)
    {
        if (*volume != '\0')
        {
            strcat(volume, ",");
        }
        for(; atts[att]; att += 2)
        {
            if(strcmp(atts[att], "resource-id") == 0)
            {
                strcat(volume, atts[att+1]);
            }
        }
    }
}

static void XMLCALL endVolumeElement(void *userData, const char *name)
{
}

int getVolumes(const char *buf, char *volumes)
{
    if (buf == NULL || *buf == '\0')
    {
        return -1;
    }
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, volumes);
    XML_SetElementHandler(parser, startVolumeElement, endVolumeElement);
    if (XML_Parse(parser, buf, strlen(buf), 1) == XML_STATUS_ERROR)
    {
        return -2;
    }
    XML_ParserFree(parser);
    return 0;
}

static void XMLCALL startVolumeDetailElement(void *userData, const char *name, const char **atts)
{
    VOLUME_INFO *v = (VOLUME_INFO *)userData;
    int att = 0;
    if (strcmp(name, "Volume") == 0)
    {
        for(; atts[att]; att += 2)
        {
            if (strcmp(atts[att], "resource-id") == 0 &&
                strcmp(atts[att+1], v->name) == 0)
            {
                v->matched = 1;
            }
        }
    }
    else if ((strcmp(name, "Capacity") == 0 || 
                strcmp(name, "Available") == 0) &&
                v->matched == 1)
    {
        strcpy(v->tag, name);
    }
}

static void XMLCALL endVolumeDetailElement(void *userData, const char *name)
{
    VOLUME_INFO *v = (VOLUME_INFO *)userData;
    if (strcmp(name, "Volume") == 0)
    {
        v->matched = 0;
    }
    strcpy(v->tag, "");
}

void XMLCALL volumeDetailText(void *userData, const XML_Char *s, int len)
{
    VOLUME_INFO *v = (VOLUME_INFO *)userData;
    if (strcmp(v->tag, "Capacity") == 0 && v->matched != 0)
    {
        strncpy(v->total, s, len);
    }
    else if (strcmp(v->tag, "Available") == 0 && v->matched != 0)
    {
        strncpy(v->free, s, len);
    }
    else if (strcmp(v->tag, "Volume_Name") == 0)
    {
        if (strncmp(v->name, s, len) == 0)
        {
            v->matched = 1;
        }
    }
}

int getVolumeDetail(const char *buf, const char *volume, char *total, char *free)
{
    if (buf == NULL || *buf == '\0')
    {
        return -1;
    }
    VOLUME_INFO v;
    memset(&v, 0, sizeof v);
    strcpy(v.name, volume);
    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetUserData(parser, &v);
    XML_SetCharacterDataHandler(parser, volumeDetailText);
    XML_SetElementHandler(parser, startVolumeDetailElement, endVolumeDetailElement);
    if (XML_Parse(parser, buf, strlen(buf), 1) == XML_STATUS_ERROR)
    {
        return -2;
    }
    XML_ParserFree(parser);
    strcpy(total, v.total);
    strcpy(free, v.free);

    return 0;
}

#if 0
int main()
{
    const char buf[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                 "<Volume_Collection>"
                 "<Volume resource-id=\"data\" resource-type=\"Volume\" ejectable=\"false\">"
                 "<Property_List>"
                 "<Volume_Name>data</Volume_Name>"
                 "<Volume_Mode>1</Volume_Mode>"
                 "<AltRoot/>"
                 "<DeDup/>"
                 "<AutoExpand>on</AutoExpand>"
                 "<AutoReplace>on</AutoReplace>"
                 "<CacheFile/>"
                 "<Delegation>on</Delegation>"
                 "<ListSnapshots>on</ListSnapshots>"
                 "<Version>0</Version>"
                 "<Allocated>2923321800</Allocated>"
                 "<Capacity>2923321800</Capacity>"
                 "<Free>2923116828</Free>"
                 "<Available>2923116828</Available>"
                 "<GUID>98ff9569-fbd7-44ba-a28c-bfc5e4c45f18</GUID>"
                 "<Health>UNPROTECTED</Health>"
                 "<Checksum>on</Checksum>"
                 "<Encryption enabled=\"0\"/>"
                 "</Property_List>"
                 "<vdev_list>"
                 "<RAID LEVEL=\"1\" ID=\"0\">"
                 "<Disk resource-id=\"sdb\"/>"
                 "</RAID>"
                 "</vdev_list>"
                 "</Volume>"
                 "</Volume_Collection>";
/*"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                 "<Volume_Collection>"
                 "<Volume resource-id=\"data\" resource-type=\"Volume\"/>"
                 "</Volume_Collection>";

    printf("buf: %s\n", buf);
    char volumes[1024] = {0};
    getVolumes(buf, volumes);
    printf("%s\n", volumes);
*/
    const char volume[] = "data";
    const char total[64] = {0};
    const char free[64] = {0};
    int ret = getVolumeDetail(buf, volume, total, free);
    printf("ret=[%d], total=[%s], free=[%s]\n", ret, total, free);

    return 0;
}
#endif


